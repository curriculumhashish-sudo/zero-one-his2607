#pragma once
#ifndef _MEDICALRECORDMAPPER_H_
#define _MEDICALRECORDMAPPER_H_

#include "Mapper.h"
#include "../../domain/do/diagnosis-entry/DiagnosisEntryDO.h"

/**
 * 示例表字段匹配映射
 */
class MedicalRecordBriefMapper : public Mapper<DiagnosisEntryBriefDO>
{
public:
	DiagnosisEntryBriefDO mapper(ResultSet* resultSet) const override
	{
		DiagnosisEntryBriefDO data;
		data.setDefinition_name(resultSet->getString(1));
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象
 */
class PtrMedicalRecordBriefMapper : public Mapper<PtrDiagnosisEntryBriefDO>
{
public:
	PtrDiagnosisEntryBriefDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<DiagnosisEntryBriefDO>();
		data->setDefinition_name(resultSet->getString(1));
		return data;
	}
};


class MedicalRecordDetailMapper : public Mapper<DiagnosisEntryDetailDO>
{
public:
	DiagnosisEntryDetailDO mapper(ResultSet* resultSet) const override
	{
		DiagnosisEntryDetailDO data;
		data.setCondition_id(resultSet->getString(1));
		data.setDefinition_code(resultSet->getString(2));
		data.setDescription(resultSet->getString(3));
		data.setKind_name(resultSet->getString(4));
		data.setType_name(resultSet->getString(5));
		data.setMain_flag(resultSet->getString(6));
		return data;
	}
};



//class MedicalRecordByIdMapper : public Mapper<MedicalRecordByIdDO>
//{
//public:
//	MedicalRecordByIdDO mapper(ResultSet* resultSet) const override
//	{
//		MedicalRecordByIdDO data;
//		data.setId(resultSet->getString(1));
//		data.setContent(resultSet->getString(2));
//		return data;
//	}
//};
//
///**
// * 示例表字段匹配映射-创建智能指针对象
// */
//class PtrMedicalRecordByIdMapper : public Mapper<PtrMedicalRecordByIdDO>
//{
//public:
//	PtrMedicalRecordByIdDO mapper(ResultSet* resultSet) const override
//	{
//		auto data = std::make_shared<MedicalRecordByIdDO>();
//		data->setId(resultSet->getString(1));
//		data->setContent(resultSet->getString(2));
//		return data;
//	}
//};










#endif // !