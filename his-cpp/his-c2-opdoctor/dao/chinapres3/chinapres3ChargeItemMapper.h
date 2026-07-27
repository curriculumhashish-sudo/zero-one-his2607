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
#ifndef _chinapres3ChargeItemMapper_H_
#define _chinapres3ChargeItemMapper_H_

#include "Mapper.h"
#include "../../domain/do/chinapres3/chinapres3ChargeItemDO.h"

/**
 * 中药处方收费记录映射器
 *
 * 数据库查询结果集 ResultSet 转换为 chinapres3ChargeItemDO 对象
 * 对应数据库表adm_charge_item
 */
class chinapres3ChargeItemMapper : public Mapper<Ptrchinapres3ChargeItemDO>
{
public:
	Ptrchinapres3ChargeItemDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<chinapres3ChargeItemDO>();
		// 主键ID
		data->setId(resultSet->getString("id"));
		// DML业务关联字段
		data->setServiceTable(resultSet->getString("service_table"));
		data->setServiceId(resultSet->getString("service_id"));
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setPrescriptionNo(resultSet->getString("prescription_no"));
		// 收费金额、数量相关字段
		data->setDefinitionId(resultSet->getString("definition_id"));
		data->setTotalPrice(resultSet->getDouble("total_price"));
		data->setQuantityValue(resultSet->getDouble("quantity_value"));
		data->setQuantityUnit(resultSet->getString("quantity_unit"));
		data->setUnitPrice(resultSet->getDouble("unit_price"));
		data->setAccountId(resultSet->getString("account_id"));
		data->setOrgId(resultSet->getString("org_id"));
		data->setConditionId(resultSet->getString("condition_id"));
		data->setEncounterDiagnosisId(resultSet->getString("encounter_diagnosis_id"));
		data->setGenerateSourceEnum(resultSet->getString("generate_source_enum"));
		data->setContextEnum(resultSet->getString("context_enum"));
		data->setCreateBy(resultSet->getString("create_by"));
		data->setCreateAt(resultSet->getString("create_at"));
		data->setUpdateBy(resultSet->getString("update_by"));
		data->setUpdateAt(resultSet->getString("update_at"));
		return data;
	}
};

#endif // !_chinapres3ChargeItemMapper_H_