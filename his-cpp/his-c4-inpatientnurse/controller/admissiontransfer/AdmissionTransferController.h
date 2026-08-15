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
#include "domain/query/admissiontransfer/BedBoardQuery.h"
#include "domain/query/admissiontransfer/DischargePendingQuery.h"
#include "domain/dto/admissiontransfer/AdmissionTransferWorkbenchDTO.h"
#include "domain/vo/admissiontransfer/AdmissionTransferWorkbenchVO.h"
#include "controller/RequestValidation.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("admissiontransfer.tag")

class AdmissionTransferController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(AdmissionTransferController);

public:
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.query-bed-board.summary"),
		queryBedBoard,
		BedBoardJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "panel",
			ZH_WORDS_GETTER("admissiontransfer.field.panel"), "ADMISSION", true);
		API_DEF_ADD_QUERY_PARAMS(String, "wardIds",
			ZH_WORDS_GETTER("admissiontransfer.field.wardIds"), "WARD-01", false);
		API_DEF_ADD_QUERY_PARAMS(String, "houseIds",
			ZH_WORDS_GETTER("admissiontransfer.field.houseIds"), "HOUSE-301", false);
		info->responses[Status::CODE_200].addExample("success", BedBoardJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/admissiontransfer/query-bed-board",
		queryBedBoard,
		BedBoardQuery,
		execQueryBedBoard(query, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.execute-bed-operation.summary"),
		executeBedOperation,
		BedOperationJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", ExecuteBedOperationDTO::createExample());
		info->responses[Status::CODE_200].addExample("success", BedOperationJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/execute-bed-operation",
		executeBedOperation,
		C4_REQUIRED_BODY_DTO(ExecuteBedOperationDTO::Wrapper, dto),
		execExecuteBedOperation(dto, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.recall-inpatient-discharge.summary"),
		recallInpatientDischarge,
		RecallInpatientDischargeJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", RecallInpatientDischargeDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", RecallInpatientDischargeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/recall-inpatient-discharge",
		recallInpatientDischarge,
		C4_REQUIRED_BODY_DTO(RecallInpatientDischargeDTO::Wrapper, dto),
		execRecallInpatientDischarge(dto, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.query-pending-executions.summary"),
		queryPendingExecutions,
		DischargePendingListJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), "337907884548657152", true);
		info->responses[Status::CODE_200].addExample(
			"success", DischargePendingListJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/admissiontransfer/query-discharge-pending-executions",
		queryPendingExecutions,
		DischargePendingQuery,
		execQueryPendingExecutions(query, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.query-pending-medications.summary"),
		queryPendingMedications,
		DischargePendingListJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), "337907884548657152", true);
		info->responses[Status::CODE_200].addExample(
			"success", DischargePendingListJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/admissiontransfer/query-discharge-pending-medications",
		queryPendingMedications,
		DischargePendingQuery,
		execQueryPendingMedications(query, authObject->getPayload()));
	// Change an inpatient encounter to the discharge-processing state.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.execute-inpatient-discharge.summary"),
		executeInpatientDischarge,
		ExecuteInpatientDischargeJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", ExecuteInpatientDischargeDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", ExecuteInpatientDischargeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/execute-inpatient-discharge",
		executeInpatientDischarge,
		C4_REQUIRED_BODY_DTO(ExecuteInpatientDischargeDTO::Wrapper, dto),
		execExecuteInpatientDischarge(dto, authObject->getPayload()));

	// Release the occupied bed after inpatient settlement is completed.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("admissiontransfer.clear-discharged-bed.summary"),
		clearDischargedBed,
		ClearDischargedBedJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", ClearDischargedBedDTO::createExample());
		info->responses[Status::CODE_200].addExample(
			"success", ClearDischargedBedJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/admissiontransfer/clear-discharged-bed",
		clearDischargedBed,
		C4_REQUIRED_BODY_DTO(ClearDischargedBedDTO::Wrapper, dto),
		execClearDischargedBed(dto, authObject->getPayload()));

private:
	BedBoardJsonVO::Wrapper execQueryBedBoard(
		const BedBoardQuery::Wrapper& query, const PayloadDTO& payload);
	BedOperationJsonVO::Wrapper execExecuteBedOperation(const ExecuteBedOperationDTO::Wrapper& dto,const PayloadDTO& payload);
	RecallInpatientDischargeJsonVO::Wrapper execRecallInpatientDischarge(const RecallInpatientDischargeDTO::Wrapper& dto,const PayloadDTO& payload);
	DischargePendingListJsonVO::Wrapper execQueryPendingExecutions(
		const DischargePendingQuery::Wrapper& query, const PayloadDTO& payload);
	DischargePendingListJsonVO::Wrapper execQueryPendingMedications(
		const DischargePendingQuery::Wrapper& query, const PayloadDTO& payload);
	ExecuteInpatientDischargeJsonVO::Wrapper execExecuteInpatientDischarge(
		const ExecuteInpatientDischargeDTO::Wrapper& dto,const PayloadDTO& payload);
	ClearDischargedBedJsonVO::Wrapper execClearDischargedBed(
		const ClearDischargedBedDTO::Wrapper& dto,const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ADMISSION_TRANSFER_CONTROLLER_
