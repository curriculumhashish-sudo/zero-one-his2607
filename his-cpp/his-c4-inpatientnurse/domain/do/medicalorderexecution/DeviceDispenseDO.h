#pragma once
#ifndef _DEVICEDISPENSEDO_
#define _DEVICEDISPENSEDO_

#include "../DoInclude.h"

class DeviceDispenseDO : public BaseDO
{
public:
    DeviceDispenseDO() : BaseDO("wkf_device_dispense") {}
};

typedef std::shared_ptr<DeviceDispenseDO> PtrDeviceDispenseDO;

#endif
