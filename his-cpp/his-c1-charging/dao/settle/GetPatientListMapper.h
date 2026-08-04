#pragma once
#include "Mapper.h"
#include "../../domain/do/settle/GetPatientListDO.h"

// 值语义Mapper，用于列表查询（返回 std::list<GetPatientListDO>）
class GetPatientListMapper : public Mapper<GetPatientListDO>
{
public:
    GetPatientListDO mapper(ResultSet* resultSet) const override
    {
        GetPatientListDO data;

        // 1. 映射 ID (SQL Alias: id)
        data.setId(resultSet->getString("id"));

        // 2. 映射住院号 (SQL Alias: bus_no) -> DO Setter: setBus_no
        data.setBus_no(resultSet->getString("bus_no"));

        // 3. 映射姓名 (SQL Alias: name) -> DO Setter: setName
        data.setName(resultSet->getString("name"));

        // 4. 映射性别 (SQL Alias: gender_enum) -> DO Setter: setGender_enum
        data.setGender_enum(resultSet->getInt("gender_enum"));

        // 5. 映射年龄 (SQL Alias: age) -> DO Setter: setAge
        data.setAge(resultSet->getInt("age"));

        // 6. 映射科室 (SQL Alias: org_name) -> DO Setter: setOrg_name
        data.setOrg_name(resultSet->getString("org_name"));

        // 7. 映射病区 (SQL Alias: ward_name) -> DO Setter: setWard_name
        data.setWard_name(resultSet->getString("ward_name"));

        // 8. 映射床位 (SQL Alias: bed_no) -> DO Setter: setBed_no
        data.setBed_no(resultSet->getString("bed_no"));

        // 9. 映射入院日期 (SQL Alias: admit_date) -> DO Setter: setAdmit_date
        data.setAdmit_date(resultSet->getString("admit_date"));

        // 10. 映射医保类型 (SQL Alias: insurance_type) -> DO Setter: setInsurance_type
        data.setInsurance_type(resultSet->getInt("insurance_type"));

        // 11. 映射医生 (SQL Alias: doctor_name) -> DO Setter: setDoctor_name
        data.setDoctor_name(resultSet->getString("doctor_name"));

        return data;
    }
};

// 指针语义Mapper - 用于需要 shared_ptr 的场景
class PtrGetPatientListMapper : public Mapper<PtrGetPatientListDO>
{
public:
    PtrGetPatientListDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<GetPatientListDO>();

        data->setId(resultSet->getString("id"));
        data->setBus_no(resultSet->getString("bus_no"));
        data->setName(resultSet->getString("name"));
        data->setGender_enum(resultSet->getInt("gender_enum"));
        data->setAge(resultSet->getInt("age"));
        data->setOrg_name(resultSet->getString("org_name"));
        data->setWard_name(resultSet->getString("ward_name"));
        data->setBed_no(resultSet->getString("bed_no"));
        data->setAdmit_date(resultSet->getString("admit_date"));
        data->setInsurance_type(resultSet->getInt("insurance_type"));
        data->setDoctor_name(resultSet->getString("doctor_name"));

        return data;
    }
};


