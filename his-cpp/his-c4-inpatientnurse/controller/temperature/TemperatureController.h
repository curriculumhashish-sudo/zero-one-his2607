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
#ifndef _TEMPERATURE_CONTROLLER_H_
#define _TEMPERATURE_CONTROLLER_H_

#include "domain/dto/temperature/TemperatureRecordDTO.h"
#include "domain/vo/temperature/TemperatureRecordVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("temperature.tags.tag")

class TemperatureController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(TemperatureController);

public:
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("temperature.init.summary"),
		getTemperatureInit,
		TemperatureInitJsonVO::Wrapper,
		API_TAG,
		info->responses[Status::CODE_200].addExample(
			"success", TemperatureInitJsonVO::createShared());
	);
	ENDPOINT(API_M_GET, "/temperature/init", getTemperatureInit, API_HANDLER_AUTH_PARAME)
	{
		API_HANDLER_RESP_VO(execGetTemperatureInit());
	}

	// Get temperature sheet data
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("temperature.query-by-date.summary"),
		getTemperatureData,
		TemperatureSheetDataJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("temperature.field.encounterId"), "337907884548657152", true);
		API_DEF_ADD_QUERY_PARAMS(String, "patientId",
			ZH_WORDS_GETTER("temperature.field.patientId"), "326940849401761792", true);
		API_DEF_ADD_QUERY_PARAMS(String, "windowRange",
			ZH_WORDS_GETTER("temperature.field.windowRange"), "7d", true);
		API_DEF_ADD_QUERY_PARAMS(String, "focusRecordDate",
			ZH_WORDS_GETTER("temperature.field.focusRecordDate"), "2026-07-24", false);
		API_DEF_ADD_QUERY_PARAMS(Int32, "pageIndex",
			ZH_WORDS_GETTER("temperature.field.pageIndex"), 1, false);
		info->responses[Status::CODE_200].addExample(
			"success", TemperatureSheetDataJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/temperature/get-data",
		getTemperatureData,
		TemperatureSheetGetDataRequestDTO,
		execGetTemperatureData(query));

	// Save temperature sheet data at a recorded time
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("temperature.save.summary"),
		saveTemperatureRecord,
		TemperatureSheetSaveJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", TemperatureSheetSaveRequestDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", TemperatureSheetSaveJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/temperature/save-record",
		saveTemperatureRecord,
		BODY_DTO(TemperatureSheetSaveRequestDTO::Wrapper, dto),
		execSaveTemperatureRecord(dto));

private:
	TemperatureInitJsonVO::Wrapper execGetTemperatureInit();
	TemperatureSheetDataJsonVO::Wrapper execGetTemperatureData(
		const TemperatureSheetGetDataRequestDTO::Wrapper& query);
	TemperatureSheetSaveJsonVO::Wrapper execSaveTemperatureRecord(
		const TemperatureSheetSaveRequestDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_TEMPERATURE_CONTROLLER_H_
