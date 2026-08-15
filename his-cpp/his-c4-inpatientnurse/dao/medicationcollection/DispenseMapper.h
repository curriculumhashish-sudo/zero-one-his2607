#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/18 22:40:00

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
#ifndef _DISPENSE_MAPPER_
#define _DISPENSE_MAPPER_

#include "Mapper.h"
#include "../../domain/do/medicationcollection/DispenseSummaryDO.h"
#include "../../domain/do/medicationcollection/DispenseDetailDO.h"

/**
 * Result mapper of the summary bill list query.
 * Column order: summary_no, status_enum, des_location_id, item_count,
 *               planned_dispense_time, dispense_time
 */
class DispenseSummaryMapper : public Mapper<DispenseSummaryDO>
{
public:
	DispenseSummaryDO mapper(ResultSet* resultSet) const override
	{
		DispenseSummaryDO data;
		data.setSummaryNo(resultSet->getString(1));
		data.setStatusEnum(resultSet->getString(2));
		data.setStatusText(resultSet->getString(3));
		data.setDesLocationId(resultSet->getString(4));
		data.setDesLocationName(resultSet->getString(5));
		data.setApplicantId(resultSet->getString(6));
		data.setApplicantName(resultSet->getString(7));
		data.setApplyTime(resultSet->getString(8));
		data.setReceiverId(resultSet->getString(9));
		data.setItemCount(resultSet->getInt(10));
		data.setPlannedDispenseTime(resultSet->getString(11));
		data.setDispenseTime(resultSet->getString(12));
		data.setVersion(resultSet->getInt64(13));
		return data;
	}
};

/**
 * Result mapper of the dispense detail query.
 * Column order: id, name, quantity, unit_code
 */
class DispenseDetailMapper : public Mapper<DispenseDetailDO>
{
public:
	DispenseDetailDO mapper(ResultSet* resultSet) const override
	{
		DispenseDetailDO data;
		data.setId(resultSet->getString(1));
		data.setMedicationId(resultSet->getString(2));
		data.setName(resultSet->getString(3));
		data.setQuantity(resultSet->getDouble(4));
		data.setUnitCode(resultSet->getString(5));
		data.setSpecification(resultSet->getString(6));
		data.setPackageDisplay(resultSet->getString(7));
		data.setUnitPrice(resultSet->getDouble(8));
		data.setTotalPrice(resultSet->getDouble(9));
		return data;
	}
};

#endif // !_DISPENSE_MAPPER_
