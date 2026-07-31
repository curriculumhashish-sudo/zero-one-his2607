#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _TEMPERATURE_SERVICE_H_
#define _TEMPERATURE_SERVICE_H_
#include "domain/dto/temperature/TemperatureRecordDTO.h"
#include "domain/vo/temperature/TemperatureRecordVO.h"

class TemperatureDAO;

class TemperatureService
{
public:
    TemperatureSheetGetDataResponseDTO::Wrapper getTemperatureData(
        const TemperatureSheetGetDataRequestDTO::Wrapper& query);

    TemperatureSheetSaveResponseDTO::Wrapper saveTemperatureRecord(
        const TemperatureSheetSaveRequestDTO::Wrapper& dto);

private:
    std::string generateBusNo(TemperatureDAO& dao);
};

#endif // !_TEMPERATURE_SERVICE_H_
