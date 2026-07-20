#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
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

#include "domain/dto/admissiontransfer/DischargeDTO.h"
#include "domain/query/admissiontransfer/PatientQueueQuery.h"
#include "domain/vo/admissiontransfer/AdmissionTransferVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("admissiontransfer.tag")

class AdmissionTransferController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(AdmissionTransferController);

public:
	// Query aggregate counts displayed above the patient queue.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.query-patient-statistics.summary"),
		queryPatientStatistics,
		PatientStatisticsJsonVO::Wrapper,
		API_TAG,
		API_DEF_QUERY_PARAM_BUILD(PatientStatisticsQuery);
		info->responses[Status::CODE_200].addExample(
			"success", PatientStatisticsJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/admissiontransfer/query-patient-statistics",
		queryPatientStatistics,
		PatientStatisticsQuery,
		execQueryPatientStatistics(query));

	// Query the inpatient nurse-station patient queue.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.query-patient-queue.summary"),
		queryPatientQueue,
		PatientQueuePageJsonVO::Wrapper,
		API_TAG,
		API_DEF_QUERY_PARAM_BUILD(PatientQueueQuery);
		info->responses[Status::CODE_200].addExample(
			"success", PatientQueuePageJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/admissiontransfer/query-patient-queue",
		queryPatientQueue,
		PatientQueueQuery,
		execQueryPatientQueue(query));

	// Change an inpatient encounter to the discharge-processing state.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.execute-inpatient-discharge.summary"),
		executeInpatientDischarge,
		DischargeJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", DischargeDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", DischargeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/execute-inpatient-discharge",
		executeInpatientDischarge,
		BODY_DTO(DischargeDTO::Wrapper, dto),
		execExecuteInpatientDischarge(dto));

	// Release the occupied bed after inpatient settlement is completed.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.clear-discharged-bed.summary"),
		clearDischargedBed,
		DischargeJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", DischargeDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", DischargeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/clear-discharged-bed",
		clearDischargedBed,
		BODY_DTO(DischargeDTO::Wrapper, dto),
		execClearDischargedBed(dto));

private:
	PatientStatisticsJsonVO::Wrapper execQueryPatientStatistics(
		const PatientStatisticsQuery::Wrapper& query);
	PatientQueuePageJsonVO::Wrapper execQueryPatientQueue(
		const PatientQueueQuery::Wrapper& query);
	DischargeJsonVO::Wrapper execExecuteInpatientDischarge(
		const DischargeDTO::Wrapper& dto);
	DischargeJsonVO::Wrapper execClearDischargedBed(
		const DischargeDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ADMISSION_TRANSFER_CONTROLLER_
