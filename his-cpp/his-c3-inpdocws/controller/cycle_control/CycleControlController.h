#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 0:27:04

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
#ifndef _CYCLE_CONTROL_CONTROLLER_H_
#define _CYCLE_CONTROL_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/cycle_control/PatientQueueQuery.h"
#include "domain/dto/cycle_control/DischargeOrderDTO.h"
#include "domain/vo/cycle_control/PatientQueuePageVO.h"
#include "domain/vo/cycle_control/DischargeOrderVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("inpdocws.cycle-control.tag")

/**
 * 住院医生站周期控制控制器。
 * 处理住院患者队列查询和出院医嘱创建。
 */
class CycleControlController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(CycleControlController);
public:
	/**
	 * 查询住院患者队列 - 分页获取在院患者列表。
	 * 支持按病区、关键字（住院号/姓名/床号）筛选。
	 */
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("inpdocws.cycle-control.patient-queue.summary"),
		queryPatientQueue, PatientQueueQuery,
		PatientQueuePageVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET, "/inpatientDoctorStation/cycle-control/patient-queue",
		queryPatientQueue, PatientQueueQuery,
		execQueryPatientQueue(query)
	);

	/**
	 * 创建出院医嘱。
	 * 向 wkf_service_request 写入一条 DISCHARGE 类型的医嘱，并将 encounter.status 更新为 ON_HOLD。
	 */
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("inpdocws.cycle-control.discharge-order.summary"),
		createDischargeOrder, DischargeOrderVO::Wrapper, API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST, "/inpatientDoctorStation/cycle-control/discharge-order",
		createDischargeOrder, BODY_DTO(CreateDischargeOrderDTO::Wrapper, dto),
		execCreateDischargeOrder(dto, authObject->getPayload())
	);

private:
	PatientQueuePageVO::Wrapper execQueryPatientQueue(const PatientQueueQuery::Wrapper& query);
	DischargeOrderVO::Wrapper execCreateDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_CYCLE_CONTROL_CONTROLLER_H_
