#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _TEMPERATURE_RECORD_VO_
#define _TEMPERATURE_RECORD_VO_

#include "../../GlobalInclude.h"
#include "../../dto/temperature/TemperatureRecordDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TemperatureSheetDataJsonVO : public JsonVO<TemperatureSheetGetDataResponseDTO::Wrapper>
{
	DTO_INIT(TemperatureSheetDataJsonVO, JsonVO<TemperatureSheetGetDataResponseDTO::Wrapper>);

public:
	TemperatureSheetDataJsonVO()
	{
		this->data = TemperatureSheetGetDataResponseDTO::createShared();
	}
};

class TemperatureSheetSaveJsonVO : public JsonVO<TemperatureSheetSaveResponseDTO::Wrapper>
{
	DTO_INIT(TemperatureSheetSaveJsonVO, JsonVO<TemperatureSheetSaveResponseDTO::Wrapper>);

public:
	TemperatureSheetSaveJsonVO()
	{
		this->data = TemperatureSheetSaveResponseDTO::createShared();
	}
};

class TemperatureInitJsonVO : public JsonVO<TemperatureInitResponseDTO::Wrapper>
{
	DTO_INIT(TemperatureInitJsonVO, JsonVO<TemperatureInitResponseDTO::Wrapper>);

public:
	TemperatureInitJsonVO()
	{
		this->data = TemperatureInitResponseDTO::createShared();
	}
};

class TemperatureSheetBatchSaveJsonVO : public JsonVO<TemperatureSheetBatchSaveResponseDTO::Wrapper>
{
	DTO_INIT(TemperatureSheetBatchSaveJsonVO, JsonVO<TemperatureSheetBatchSaveResponseDTO::Wrapper>);

public:
	TemperatureSheetBatchSaveJsonVO()
	{
		this->data = TemperatureSheetBatchSaveResponseDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TEMPERATURE_RECORD_VO_
