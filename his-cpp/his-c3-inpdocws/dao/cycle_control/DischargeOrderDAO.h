#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:23:49

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
#ifndef _DISCHARGE_ORDER_DAO_H_
#define _DISCHARGE_ORDER_DAO_H_

#include "BaseDAO.h"
#include "../../domain/dto/cycle_control/DischargeOrderDTO.h"

/**
 * 出院医嘱DAO。
 * 向wkf_service_request插入DISCHARGE类型的医嘱记录，并更新就诊状态。
 */
class DischargeOrderDAO : public BaseDAO
{
public:
	// 向wkf_service_request插入出院医嘱记录，返回新记录ID
	string insertDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto);
	// 更新就诊状态为ON_HOLD
	int updateEncounterStatus(const string& encounterId);
};

#endif // !_DISCHARGE_ORDER_DAO_H_
