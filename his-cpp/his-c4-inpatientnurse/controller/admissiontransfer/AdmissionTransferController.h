#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#ifndef _ADMISSION_TRANSFER_CONTROLLER_
#define _ADMISSION_TRANSFER_CONTROLLER_

#include "domain/dto/admissiontransfer/ClearDischargedBedDTO.h"
#include "domain/dto/admissiontransfer/ExecuteInpatientDischargeDTO.h"
#include "domain/vo/admissiontransfer/ClearDischargedBedVO.h"
#include "domain/vo/admissiontransfer/ExecuteInpatientDischargeVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("admissiontransfer.tag")

class AdmissionTransferController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(AdmissionTransferController);

public:
	// Change an inpatient encounter to the discharge-processing state.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.execute-inpatient-discharge.summary"),
		executeInpatientDischarge,
		ExecuteInpatientDischargeJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", ExecuteInpatientDischargeDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", ExecuteInpatientDischargeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/execute-inpatient-discharge",
		executeInpatientDischarge,
		BODY_DTO(ExecuteInpatientDischargeDTO::Wrapper, dto),
		execExecuteInpatientDischarge(dto, authObject->getPayload()));

	// Release the occupied bed after inpatient settlement is completed.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.clear-discharged-bed.summary"),
		clearDischargedBed,
		ClearDischargedBedJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", ClearDischargedBedDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", ClearDischargedBedJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/clear-discharged-bed",
		clearDischargedBed,
		BODY_DTO(ClearDischargedBedDTO::Wrapper, dto),
		execClearDischargedBed(dto, authObject->getPayload()));

private:
	ExecuteInpatientDischargeJsonVO::Wrapper execExecuteInpatientDischarge(
		const ExecuteInpatientDischargeDTO::Wrapper& dto,const PayloadDTO& payload);
	ClearDischargedBedJsonVO::Wrapper execClearDischargedBed(
		const ClearDischargedBedDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ADMISSION_TRANSFER_CONTROLLER_
