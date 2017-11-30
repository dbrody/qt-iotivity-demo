
#include <QDebug>

#include "iotclient.h"
#include "namedefs.h"

IoTClient::IoTClient()
{
    qDebug() << "Running IoTClient constructor";
    initializePlatform();
}

IoTClient::~IoTClient()
{
    qDebug() << "Running IoTClient destructor";
}

void IoTClient::initializePlatform()
{
    PlatformConfig cfg
    {
        ServiceType::InProc,
        ModeType::Client,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        QualityOfService::LowQos
    };
    OCPlatform::Configure(cfg);
    m_resourceDiscoveryCallback = bind(&IoTClient::discoveredResource, this, placeholders::_1);
}

void IoTClient::findResource()
{
    string coap_multicast_discovery = string(OC_RSRVD_WELL_KNOWN_URI);
    OCPlatform::findResource("", coap_multicast_discovery.c_str(),  CT_DEFAULT, m_resourceDiscoveryCallback,
                             OC::QualityOfService::LowQos);
}

void IoTClient::discoveredResource(shared_ptr<OCResource> Resource)
{
    try
    {
        if (Resource)
        {
            string resourceUri = Resource->uri();
            string hostAddress = Resource->host();

            cout << "\nFound Resource" << endl << "Resource Types:" << endl;
            for (auto& resourceTypes : Resource->getResourceTypes())
            {
                cout << "\t" << resourceTypes << endl;
            }

            cout << "Resource Interfaces: " << endl;
            for (auto& resourceInterfaces : Resource->getResourceInterfaces())
            {
                cout << "\t" << resourceInterfaces << endl;
            }
            cout << "Resource uri: " << resourceUri << endl;
            cout << "host: " << hostAddress << endl;

            if (resourceUri == SWITCH_ENDPOINT)
            {
                qDebug() << "Switch Endpoint found.";
                m_switchSensor = make_shared<Switch>(Resource);
                emit resourceDiscovered();
            }
        }
    }
    catch (OCException& ex)
    {
        cerr << "Caught exception in discoveredResource: " << ex.reason() << endl;
    }
}

shared_ptr<Switch> IoTClient::getSwitchSensor()
{
    return m_switchSensor;
}

Switch::Switch(shared_ptr<OCResource> Resource)
{
    m_resourceHandle = Resource;
    m_GETCallback = bind(&Switch::onGet, this, placeholders::_1, placeholders::_2, placeholders::_3);
    m_PUTCallback = bind(&Switch::onPut, this, placeholders::_1, placeholders::_2, placeholders::_3);
}

Switch::~Switch()
{

}

shared_ptr<OCResource> Switch::getHandle()
{
    return m_resourceHandle;
}

void Switch::get()
{
    QueryParamsMap params;
    m_resourceHandle->get(params, m_GETCallback);
}

void Switch::put(bool value)
{
    QueryParamsMap params;
    OCRepresentation rep;
    rep.setValue(SWITCH_RESOURCE_KEY, value);
    m_resourceHandle->put(rep, params, m_PUTCallback);
}

void Switch::onGet(const HeaderOptions& headerOptions, const OCRepresentation& representation,
                int errCode)
{
    if (errCode == OC_STACK_OK)
    {
        int value;
        representation.getValue(SWITCH_RESOURCE_KEY, value);
        cout << endl << endl << "Switch state is: " << value << endl;
    }
    else {
        cerr << endl << endl << "Error in GET response from LED resource: " << errCode << endl;
        QString codeStr = QVariant::fromValue(errCode).value<QString>();
        qDebug() << codeStr;
    }
}

void Switch::onPut(const HeaderOptions& headerOptions, const OCRepresentation& representation,
                int errCode)
{
    if (errCode == OC_STACK_OK)
    {
        int value;
        representation.getValue(SWITCH_RESOURCE_KEY, value);
        cout << endl << endl << "Set LED switch to: " << value << endl;
    }
    else {
        cerr << endl << endl << "Error in PUT response from LED resource: " << errCode << endl;

        QString codeStr = QVariant::fromValue(errCode).value<QString>();
        qDebug() << codeStr;
    }
}
