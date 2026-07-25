#pragma once
#ifndef _DEVICEREQUESTDO_
#define _DEVICEREQUESTDO_

#include "../DoInclude.h"

class DeviceRequestDO : public BaseDO
{
public:
    DeviceRequestDO() : BaseDO("wkf_device_request") {}
};

typedef std::shared_ptr<DeviceRequestDO> PtrDeviceRequestDO;

#endif
