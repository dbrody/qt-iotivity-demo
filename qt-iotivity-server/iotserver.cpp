
#include <signal.h>
#include <thread>
#include <functional>
#include <QDebug>

#include "iotserver.h"
#include "namedefs.h"

// Secure Virtual Resource database for Iotivity Server
// It contains Server's Identity and the PSK credentials
// of other devices which the server trusts
static char CredFile[] = "ElevatorServerSecurityDB.dat";

FILE* server_fopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(CredFile, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};

IoTServer::IoTServer() :
    m_switchResource(nullptr)
{
    initializePlatform();
    setupResources();
    m_switchRepresentation.setUri(SWITCH_ENDPOINT);
    m_switchRepresentation.setValue((string)SWITCH_RESOURCE_KEY, false);
}

void IoTServer::initializePlatform()
{
    qDebug() << "Running IoTServer constructor";
    PlatformConfig cfg
    {
        ServiceType::InProc,
        ModeType::Server,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        56111,         // Uses randomly available port
        QualityOfService::HighQos,
        &ps
    };
    OCPlatform::Configure(cfg);
    OC_VERIFY(OCPlatform::start() == OC_STACK_OK);

    // Start Presense
    if(OCStartPresence(0) == OC_STACK_OK){
        qDebug() << "OCStartPresence Started.";
    }

    // Device Info.
    char devName[] = "Waterwize Station Controller";
    char resTypeName[] = "x.org.iotivity.waterwize.controller";
    OCStringLL types { nullptr, resTypeName };
    char specVersion[] = "0.0.1";
    OCDeviceInfo deviceInfo = { devName, &types, specVersion, nullptr };

    // Platform Info
    char platformId[] = "6cb6c994-8c4b-11e6-ae22-56b6b6499611";
    char manufacturerName[] = "E Manufacturer";
    char manufacturerUrl[] = "http://www.example.com/elevator";
    char modelNumber[] = "Elevator Model Number";
    char dateManufacture[] = "2017-02-28";
    char platformVersion[] = "Elevator Platform Version";
    char osVersion[] = "Elevator OS Version";
    char hardwareVersion[] = "Elevator HW Version";
    char firmwareVersion[] = "Elevator FW Version";
    char supportURL[] = "http://www.example.com/elevator/support";

    OCPlatformInfo platformInfo = {
        platformId,
        manufacturerName,
        manufacturerUrl,
        modelNumber,
        dateManufacture,
        platformVersion,
        osVersion,
        hardwareVersion,
        firmwareVersion,
        supportURL,
        nullptr
    };

    // Register elevator's platformInfo, deviceInfo, and resource.
    if (OC_STACK_OK == OCPlatform::registerPlatformInfo(platformInfo))
    {
        qDebug() << "Server registered platform info.";
    }

    if (OC_STACK_OK == OCPlatform::registerDeviceInfo(deviceInfo))
    {
        qDebug() << "Server registered device info.";
    }

    std::string platformDiscoveryURI = "/oic/p";
    std::ostringstream platformDiscoveryRequest;
    platformDiscoveryRequest << OC_MULTICAST_PREFIX << platformDiscoveryURI;

    qDebug() << "here";
    FindPlatformCallback cb1 = bind(&IoTServer::ReceivedPlatformInfo, this, PH::_1);
}

IoTServer::~IoTServer()
{
    cout << "Running IoTServer destructor" << endl;
}

void IoTServer::setupResources()
{
    EntityHandler cb1 = bind(&IoTServer::SwitchEntityHandler, this, PH::_1);
    OCStackResult result = createResource(SWITCH_ENDPOINT, "oic.if.a", cb1, m_switchResource);
    if (result == OC_STACK_OK)
    {
        const char* deviceID = OCGetServerInstanceIDString();
        std::cout << "Elevator server's device ID: ";
        if (deviceID != nullptr)
        {
             std::cout << deviceID << std::endl;
        }
        else
        {
            std::cout << "Unknown" << std::endl;
        }
    }
}

OCStackResult IoTServer::createResource(string Uri, string Type, EntityHandler Cb, OCResourceHandle& Handle)
{
    string resourceUri = Uri;
    string resourceType = Type;
    string resourceInterface = OC_RSRVD_INTERFACE_READ_WRITE;
    uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;

    OCStackResult result = OCPlatform::registerResource(Handle, resourceUri, resourceType,
                                                        resourceInterface, Cb, resourceFlag);

    if (result != OC_STACK_OK)
        cerr << "Could not create " << Type << " resource" << endl;
    else
        cout << "Successfully created " << Uri << " : " << Type << " resource" << endl;
    return result;
}

void IoTServer::putSwitchRepresentation()
{
    bool state = false;
    m_switchRepresentation.getValue(SWITCH_RESOURCE_KEY, state);
    if (state == false)
        cout << "Turned off LED" << endl;
    else if (state == true)
        cout << "Turned on LED" << endl;
    else
        cerr << "Invalid request value" << endl;
}

OCRepresentation IoTServer::getSwitchRepresentation()
{
    return m_switchRepresentation;
}

// Return all properties in response.
OCStackResult IoTServer::SendResponse(std::shared_ptr<OCResourceRequest> request)
{
    // Values to return.
    OCRepresentation responseRep;
    responseRep["x.org.iotivity.value"] = false;

    // Prepare the response.
    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(request->getRequestHandle());
    pResponse->setResourceHandle(request->getResourceHandle());
    pResponse->setResourceRepresentation(responseRep);
    pResponse->setResponseResult(OC_EH_OK);

    // Send the response.
    return OCPlatform::sendResponse(pResponse);
}

OCEntityHandlerResult IoTServer::SwitchEntityHandler(const shared_ptr<OCResourceRequest> request)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(request != nullptr)
    {
        // Get the request type and request flag
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if(requestFlag & RequestHandlerFlag::RequestFlag)
        {
            // If the request type is GET
            if(requestType == "GET")
            {
                if (SendResponse(request) == OC_STACK_OK)
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "POST")
            {
                OCRepresentation requestRep = request->getResourceRepresentation();

                // Target floor can be set.
                int targetFloor;
                if (requestRep.getValue("x.org.iotivity.TargetFloor", targetFloor))
                {
//                    SetTargetFloor(static_cast<int>(targetFloor));
                }

                if(OC_STACK_OK == SendResponse(request))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "PUT")
            {
                // not supported.
            }
            else if(requestType == "DELETE")
            {
                // not supported.
            }
        }

        if(requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            // Hold the lock to make sure no iterator is in progress.
//            std::lock_guard<std::mutex> lock(m_elevatorMutex);

//            ObservationInfo observationInfo = request->getObservationInfo();
//            if(ObserveAction::ObserveRegister == observationInfo.action)
//            {
//                OIC_LOG_V(INFO, TAG, "ElevatorEntityHandler(): new observer ID: %d",
//                    observationInfo.obsId);
//                m_observers.push_back(observationInfo.obsId);
//            }
//            else if(ObserveAction::ObserveUnregister == observationInfo.action)
//            {
//                OIC_LOG_V(INFO, TAG, "ElevatorEntityHandler(): removing observer ID: %d",
//                    observationInfo.obsId);
//                m_observers.erase(std::remove(
//                                    m_observers.begin(),
//                                    m_observers.end(),
//                                    observationInfo.obsId),
//                                    m_observers.end());
//            }

            ehResult = OC_EH_OK;
        }
    }

    return ehResult;
}


//Callback after device information is received
void IoTServer::ReceivedPlatformInfo(const OCRepresentation& rep)
{
    std::cout << "\nPlatform Information received ---->\n";
    std::string value;
    std::string values[] =
    {
        "pi",   "Platform ID                    ",
        "mnmn", "Manufacturer name              ",
        "mnml", "Manufacturer url               ",
        "mnmo", "Manufacturer Model No          ",
        "mndt", "Manufactured Date              ",
        "mnpv", "Manufacturer Platform Version  ",
        "mnos", "Manufacturer OS version        ",
        "mnhw", "Manufacturer hardware version  ",
        "mnfv", "Manufacturer firmware version  ",
        "mnsl", "Manufacturer support url       ",
        "st",   "Manufacturer system time       "
    };

    for (unsigned int i = 0; i < sizeof(values) / sizeof(values[0]) ; i += 2)
    {
        if(rep.getValue(values[i], value))
        {
            std::cout << values[i + 1] << " : "<< value << std::endl;
        }
    }
}
