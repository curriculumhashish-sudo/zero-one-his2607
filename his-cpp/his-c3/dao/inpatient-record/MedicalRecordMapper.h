#pragma once
#ifndef _MEDICALRECORDMAPPER_H_
#define _MEDICALRECORDMAPPER_H_

#include "Mapper.h"
#include "../../domain/do/inpatient-record/MedicalRecordDO.h"

/**
 * 示例表字段匹配映射
 */
class MedicalRecordByTitleMapper : public Mapper<MedicalRecordByTitleDO>
{
public:
	MedicalRecordByTitleDO mapper(ResultSet* resultSet) const override
	{
		MedicalRecordByTitleDO data;
		data.setId(resultSet->getString(1));
		data.setDocument_type_name(resultSet->getString(2));
		data.setTitle(resultSet->getString(3));
		data.setStatus(resultSet->getString(4));
		data.setUpdated_at(resultSet->getString(5));
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象
 */
class PtrMedicalRecordByTitleMapper : public Mapper<PtrMedicalRecordByTitleDO>
{
public:
	PtrMedicalRecordByTitleDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<MedicalRecordByTitleDO>();
		data->setId(resultSet->getString(1));
		data->setDocument_type_name(resultSet->getString(2));
		data->setTitle(resultSet->getString(3));
		data->setStatus(resultSet->getString(4));
		data->setUpdated_at(resultSet->getString(5));
		return data;
	}
};



class MedicalRecordByIdMapper : public Mapper<MedicalRecordByIdDO>
{
public:
	MedicalRecordByIdDO mapper(ResultSet* resultSet) const override
	{
		MedicalRecordByIdDO data;
		data.setId(resultSet->getString(1));
		data.setContent(resultSet->getString(2));
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象
 */
class PtrMedicalRecordByIdMapper : public Mapper<PtrMedicalRecordByIdDO>
{
public:
	PtrMedicalRecordByIdDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<MedicalRecordByIdDO>();
		data->setId(resultSet->getString(1));
		data->setContent(resultSet->getString(2));
		return data;
	}
};










#endif // !