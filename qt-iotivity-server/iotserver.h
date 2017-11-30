#ifndef IOTSERVER_H
#define IOTSERVER_H

#include <string>
#include <iostream>
#include <memory>

#include <ocstack.h>
#include <OCPlatform.h>
#include <OCApi.h>

using namespace std;
using namespace OC;
namespace PH = std::placeholders;

class IoTServer
{
    shared_ptr<PlatformConfig> m_platformConfig;
    OCRepresentation m_switchRepresentation;
    OCResourceHandle m_switchResource;

    void initializePlatform();
    void setupResources();
    OCStackResult createResource(string Uri, string Type, EntityHandler Cb, OCResourceHandle& Handle);

    OCRepresentation getSwitchRepresentation();
    void putSwitchRepresentation();
    OCEntityHandlerResult SwitchEntityHandler(shared_ptr<OCResourceRequest> request);

public:
    IoTServer();
    virtual ~IoTServer();
    void ReceivedPlatformInfo(const OCRepresentation& rep);
    OCStackResult SendResponse(std::shared_ptr<OCResourceRequest> request);
};

#endif // IOTSERVER_H
