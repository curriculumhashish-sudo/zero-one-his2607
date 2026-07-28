// ======================================================
// GetPatientListMapper.h - 患者列表 - 结果集映射器
// Mapper 的工作流程：
// 1. DAO 执行 SQL → 产生 ResultSet（N行）
// 2. DAO 调用 sqlSession->executeQuery<GetPatientListDO>(sql, GetPatientListMapper(), params)
// 3. 框架对 ResultSet 每行调用 mapper(resultSet) → 返回一个 GetPatientListDO
// 4. 所有 DO 收集到 std::list<GetPatientListDO> 返回给 DAO
// ======================================================
#pragma once
#include "Mapper.h"
#include "../../domain/do/settle/GetPatientListDO.h"

// 值语义Mapper，用于列表查询 (返回 std::list<GetPatientListDO>)
class GetPatientListMapper : public Mapper<GetPatientListDO>
{
public:
	GetPatientListDO mapper(ResultSet* resultSet) const override
	{
		GetPatientListDO data;
		data.setEncounter_id(resultSet->getString("encounter_id"));
		data.setInpatient_no(resultSet->getString("inpatient_no"));
		data.setPatient_name(resultSet->getString("patient_name"));
		data.setGender(resultSet->getInt("gender"));
		data.setAge(resultSet->getInt("age"));
		data.setDept_name(resultSet->getString("dept_name"));
		data.setWard_name(resultSet->getString("ward_name"));
		data.setBed_no(resultSet->getString("bed_no"));
		data.setAdmit_date(resultSet->getString("admit_date"));
		data.setInsurance_type(resultSet->getInt("insurance_type"));
		data.setDoctor_name(resultSet->getString("doctor_name"));
		return data;
	}
};

// 指针语义Mapper – 用于需要 shared_ptr 的场景
class PtrGetPatientListMapper : public Mapper<PtrGetPatientListDO>
{
public:
	PtrGetPatientListDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetPatientListDO>();
		data->setEncounter_id(resultSet->getString("encounter_id"));
		data->setInpatient_no(resultSet->getString("inpatient_no"));
		data->setPatient_name(resultSet->getString("patient_name"));
		data->setGender(resultSet->getInt("gender"));
		data->setAge(resultSet->getInt("age"));
		data->setDept_name(resultSet->getString("dept_name"));
		data->setWard_name(resultSet->getString("ward_name"));
		data->setBed_no(resultSet->getString("bed_no"));
		data->setAdmit_date(resultSet->getString("admit_date"));
		data->setInsurance_type(resultSet->getInt("insurance_type"));
		data->setDoctor_name(resultSet->getString("doctor_name"));
		return data;
	}
};

