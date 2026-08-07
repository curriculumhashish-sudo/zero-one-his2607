#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2025/07/16 15:47:39

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
#ifndef _COSTINFO_MAPPER_H_
#define _COSTINFO_MAPPER_H_
#include "Mapper.h"
#include "domain/do/cost_info/costInfoDO.h"

class CostInfoMapper : public Mapper<CostInfoDO>
{
public:
	CostInfoDO mapper(ResultSet* resultSet) const override
	{
		CostInfoDO data;
		data.setChargeID(resultSet->getString("chargeID"));
		data.setEncounterID(resultSet->getString("encounterID"));
		data.setPaymentID(resultSet->getString("paymentID"));
		data.setCashierID(resultSet->getString("cashierID"));
		data.setName(resultSet->getString("name"));
		data.setRoom(resultSet->getString("room"));
		data.setDate(resultSet->getString("date"));
		data.setFeeClass(resultSet->getString("feeClass"));
		data.setDetailClass(resultSet->getString("detailClass"));
		data.setProjectName(resultSet->getString("projectName"));
		data.setHisID(resultSet->getString("hisID"));
		data.setSpec(resultSet->getString("spec"));
		data.setNumber(resultSet->getDouble("number"));
		data.setUnit(resultSet->getString("unit"));
		data.setDoctor(resultSet->getString("doctor"));
		data.setChecker(resultSet->getString("checker"));
		data.setSettleTime(resultSet->getString("settleTime"));
		data.setSource(resultSet->getString("source"));
		data.setInsuranceLevel(resultSet->getString("insuranceLevel"));
		data.setInsuranceType(resultSet->getString("insuranceType"));
		data.setOriginalPrice(resultSet->getDouble("originalPrice"));
		data.setDiscountAmount(resultSet->getDouble("discountAmount"));
		data.setReceivableAmount(resultSet->getDouble("receivableAmount"));
		data.setDiscountedPrice(resultSet->getDouble("discountedPrice"));
		data.setUnitPrice(resultSet->getDouble("unitPrice"));
		data.setLineAmount(resultSet->getDouble("lineAmount"));
		data.setActualAmount(resultSet->getDouble("actualAmount"));
		data.setInsuranceAmount(resultSet->getDouble("insuranceAmount"));
		data.setSelfPayAmount(resultSet->getDouble("selfPayAmount"));
		return data;
	}
};

class PtrCostInfoMapper : public Mapper<PtrCostInfoDO>
{
public:
	PtrCostInfoDO mapper(ResultSet* resultSet) const override
	{
		PtrCostInfoDO file = std::make_shared<CostInfoDO>();
		file->setChargeID(resultSet->getString("chargeID"));
		file->setEncounterID(resultSet->getString("encounterID"));
		file->setPaymentID(resultSet->getString("paymentID"));
		file->setCashierID(resultSet->getString("cashierID"));
		file->setName(resultSet->getString("name"));
		file->setRoom(resultSet->getString("room"));
		file->setDate(resultSet->getString("date"));
		file->setFeeClass(resultSet->getString("feeClass"));
		file->setDetailClass(resultSet->getString("detailClass"));
		file->setProjectName(resultSet->getString("projectName"));
		file->setHisID(resultSet->getString("hisID"));
		file->setSpec(resultSet->getString("spec"));
		file->setNumber(resultSet->getDouble("number"));
		file->setUnit(resultSet->getString("unit"));
		file->setDoctor(resultSet->getString("doctor"));
		file->setChecker(resultSet->getString("checker"));
		file->setSettleTime(resultSet->getString("settleTime"));
		file->setSource(resultSet->getString("source"));
		file->setInsuranceLevel(resultSet->getString("insuranceLevel"));
		file->setInsuranceType(resultSet->getString("insuranceType"));
		file->setOriginalPrice(resultSet->getDouble("originalPrice"));
		file->setDiscountAmount(resultSet->getDouble("discountAmount"));
		file->setReceivableAmount(resultSet->getDouble("receivableAmount"));
		file->setDiscountedPrice(resultSet->getDouble("discountedPrice"));
		file->setUnitPrice(resultSet->getDouble("unitPrice"));
		file->setLineAmount(resultSet->getDouble("lineAmount"));
		file->setActualAmount(resultSet->getDouble("actualAmount"));
		file->setInsuranceAmount(resultSet->getDouble("insuranceAmount"));
		file->setSelfPayAmount(resultSet->getDouble("selfPayAmount"));
		return file;
	}
};

#endif // !_COSTINFO_MAPPER_H_