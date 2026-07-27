#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2025/07/26

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
#ifndef _chinapres3MedicationRequestMapper_H_
#define _chinapres3MedicationRequestMapper_H_

#include "Mapper.h"
#include "../../domain/do/chinapres3/chinapres3MedicationRequestDO.h"

/**
 * 中药处方单据映射器
 *
 * 数据库查询结果集 ResultSet 转换为 chinapres3MedicationRequestDO 对象
 * 对应数据库表med_medication_request
 */
class chinapres3MedicationRequestMapper : public Mapper<Ptrchinapres3MedicationRequestDO>
{
public:
	Ptrchinapres3MedicationRequestDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<chinapres3MedicationRequestDO>();
		// 主键ID
		data->setId(resultSet->getString("id"));
		// DML业务关联字段
		data->setPrescriptionNo(resultSet->getString("prescription_no"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		// 处方类型、创建更新人时间字段
		data->setRxTypeCode(resultSet->getString("rx_type_code"));
		data->setCreateBy(resultSet->getString("create_by"));
		data->setCreateAt(resultSet->getString("create_at"));
		data->setUpdateBy(resultSet->getString("update_by"));
		data->setUpdateAt(resultSet->getString("update_at"));
		return data;
	}
};

#endif // !_chinapres3MedicationRequestMapper_H_