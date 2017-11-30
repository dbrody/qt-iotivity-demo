#ifndef IOTCLIENT_H
#define IOTCLIENT_H

#include <QObject>
#include <string>
#include <iostream>
#include <memory>

#include <ocstack.h>
#include <OCPlatform.h>
#include <OCApi.h>
#include <OCResource.h>

using namespace std;
using namespace OC;

class Switch
{
    shared_ptr<OCResource> m_resourceHandle;
    OCRepresentation m_switchRepresentation;
    GetCallback m_GETCallback;
    PutCallback m_PUTCallback;

public:
    void get();
    void put(bool);

    void onGet(const HeaderOptions&, const OCRepresentation&, int);
    void onPut(const HeaderOptions&, const OCRepresentation&, int);
    shared_ptr<OCResource> getHandle();
    Switch(shared_ptr<OCResource> Resource);
    virtual ~Switch();
};

class IoTClient : public QObject
{
    Q_OBJECT

    shared_ptr<Switch> m_switchSensor;
    shared_ptr<PlatformConfig> m_platformConfig;
    FindCallback m_resourceDiscoveryCallback;
    void initializePlatform();
    void discoveredResource(shared_ptr<OCResource>);

public:
    shared_ptr<Switch> getSwitchSensor();
    void findResource();
    IoTClient();
    virtual ~IoTClient();

signals:
    void resourceDiscovered();
};

#endif // IOTCLIENT_H
