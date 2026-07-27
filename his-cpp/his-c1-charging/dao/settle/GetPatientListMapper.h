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
		data.setEncounter_id(resultSet->getLongLong("encounter_id"));
		data.setInpatient_no(resultSet->getString("inpatient_no"));
		data.setPatient_name(resultSet->getString("patient_name"));
		data.setGender(resultSet->getInt("gender"));
		data.setAge(resultSet->getInt("age"));
		data.setDept_name(resultSet->getString("dept_name"));
		data.setWard_name(resultSet->getString("ward_name"));
		data.setBed_no(resultSet->getString("bed_no"));
		data.setAdmit_date(resultSet->getLongLong("admit_date"));
		data.setInsurance_type(resultSet->getInt("insurance_type"));
		data.setEncounter_status(resultSet->getInt("encounter_status"));
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
		data->setEncounter_id(resultSet->getLongLong("encounter_id"));
		data->setInpatient_no(resultSet->getString("inpatient_no"));
		data->setPatient_name(resultSet->getString("patient_name"));
		data->setGender(resultSet->getInt("gender"));
		data->setAge(resultSet->getInt("age"));
		data->setDept_name(resultSet->getString("dept_name"));
		data->setWard_name(resultSet->getString("ward_name"));
		data->setBed_no(resultSet->getString("bed_no"));
		data->setAdmit_date(resultSet->getLongLong("admit_date"));
		data->setInsurance_type(resultSet->getInt("insurance_type"));
		data->setEncounter_status(resultSet->getInt("encounter_status"));
		return data;
	}
};
