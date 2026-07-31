#pragma once

#ifndef _TREATMENT_MAPPER_
#define _TREATMENT_MAPPER_

#include "Mapper.h"
#include "../../domain/do/treatment/WkfActivityDefinitionDo.h"
#include "../../domain/do/treatment/WkfActivityDefinitionChildrenDo.h"
#include "../../domain/do/treatment/AdmEpisodeOfCareEncounterDO.h"
#include "../../domain/do/treatment/AdmEpisodeOfCareDO.h"

/**
 * 诊疗项目主表 普通值对象Mapper
 */
class TreatmentMapper : public Mapper<TreatmentDO>
{
public:
	TreatmentDO mapper(ResultSet* resultSet) const override
	{
		TreatmentDO data;
		// 1 主键id
		data.setId(resultSet->getString(1));

		// 基础审计、乐观锁字段
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getInt(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));

		// 业务诊疗字段依次赋值
		data.setCategoryCode(resultSet->getString(11));
		data.setBusNo(resultSet->getString(12));
		data.setName(resultSet->getString(13));
		data.setPyStr(resultSet->getString(14));
		data.setWbStr(resultSet->getString(15));
		data.setPackageUnitCode(resultSet->getString(16));
		data.setTypeEnum(resultSet->getString(17));
		data.setStatusEnum(resultSet->getString(18));
		data.setDescriptionText(resultSet->getString(19));
		data.setBodySiteCode(resultSet->getString(20));
		data.setSpecimenCode(resultSet->getString(21));
		data.setOrgId(resultSet->getString(22));
		data.setLocationId(resultSet->getString(23));
		data.setRuleId(resultSet->getInt(24));
		data.setPricingFlag(resultSet->getInt(25));
		data.setYbFlag(resultSet->getInt(26));
		data.setYbNo(resultSet->getString(27));
		data.setYbMatchFlag(resultSet->getInt(28));
		data.setChrgitmLv(resultSet->getInt(29));
		data.setChildrenJson(resultSet->getString(30));
		data.setChildrenFlag(resultSet->getInt(31));
		data.setActivityVersion(resultSet->getString(32));
		data.setMerchandiseName(resultSet->getString(33));
		data.setMerchandisePyStr(resultSet->getString(34));
		data.setMerchandiseWbStr(resultSet->getString(35));
		data.setDefEncounterUnitCode(resultSet->getString(36));
		data.setDefInhospitalUnitCode(resultSet->getString(37));

		return data;
	}
};

/**
 * 诊疗项目主表 智能指针Mapper
 */
class PtrTreatmentMapper : public Mapper<PtrTreatmentDO>
{
public:
	PtrTreatmentDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<TreatmentDO>();
		data->setId(resultSet->getString(1));

		data->setTenantId(resultSet->getString(2));
		data->setCreatedAt(resultSet->getString(3));
		data->setUpdatedAt(resultSet->getString(4));
		data->setCreatedBy(resultSet->getString(5));
		data->setUpdatedBy(resultSet->getString(6));
		data->setIsDeleted(resultSet->getInt(7));
		data->setDeletedAt(resultSet->getString(8));
		data->setDeletedBy(resultSet->getString(9));
		data->setVersion(resultSet->getInt(10));

		data->setCategoryCode(resultSet->getString(11));
		data->setBusNo(resultSet->getString(12));
		data->setName(resultSet->getString(13));
		data->setPyStr(resultSet->getString(14));
		data->setWbStr(resultSet->getString(15));
		data->setPackageUnitCode(resultSet->getString(16));
		data->setTypeEnum(resultSet->getString(17));
		data->setStatusEnum(resultSet->getString(18));
		data->setDescriptionText(resultSet->getString(19));
		data->setBodySiteCode(resultSet->getString(20));
		data->setSpecimenCode(resultSet->getString(21));
		data->setOrgId(resultSet->getString(22));
		data->setLocationId(resultSet->getString(23));
		data->setRuleId(resultSet->getInt(24));
		data->setPricingFlag(resultSet->getInt(25));
		data->setYbFlag(resultSet->getInt(26));
		data->setYbNo(resultSet->getString(27));
		data->setYbMatchFlag(resultSet->getInt(28));
		data->setChrgitmLv(resultSet->getInt(29));
		data->setChildrenJson(resultSet->getString(30));
		data->setChildrenFlag(resultSet->getInt(31));
		data->setActivityVersion(resultSet->getString(32));
		data->setMerchandiseName(resultSet->getString(33));
		data->setMerchandisePyStr(resultSet->getString(34));
		data->setMerchandiseWbStr(resultSet->getString(35));
		data->setDefEncounterUnitCode(resultSet->getString(36));
		data->setDefInhospitalUnitCode(resultSet->getString(37));

		return data;
	}
};

#endif // !_TREATMENT_MAPPER_