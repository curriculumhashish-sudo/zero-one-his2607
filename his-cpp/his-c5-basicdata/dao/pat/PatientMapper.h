#pragma once
#ifndef _PATIENT_MAPPER_H_
#define _PATIENT_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/pat/PatientDO.h"

/** Maps the patient identifier for list endpoints. */
class PtrPatientSelectMapper : public Mapper<PtrPatientDO>
{
public:
    PtrPatientDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<PatientDO>();
        data->setId(resultSet->getString("id"));
        data->setBusNo(resultSet->getString("bus_no"));
        data->setName(resultSet->getString("name"));
        data->setGender(resultSet->getString("gender_enum"));
        if (!resultSet->isNull("birth_date")) data->setBirthDate(resultSet->getString("birth_date"));
        if (!resultSet->isNull("phone")) data->setPhone(resultSet->getString("phone"));
        if (!resultSet->isNull("id_card")) data->setIdCard(resultSet->getString("id_card"));
        return data;
    }
};

/** Maps the patient management list projection. */
class PtrPatientPageMapper : public Mapper<PtrPatientDO>
{
public:
    PtrPatientDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<PatientDO>();
        data->setId(resultSet->getString("id"));
        data->setName(resultSet->getString("name"));
        data->setGender(resultSet->getString("gender_enum"));
        if (!resultSet->isNull("birth_date")) data->setBirthDate(resultSet->getString("birth_date"));
        if (!resultSet->isNull("phone")) data->setPhone(resultSet->getString("phone"));
        if (!resultSet->isNull("id_card")) data->setIdCard(resultSet->getString("id_card"));
        return data;
    }
};

/** Maps a complete adm_patient row for detail and update operations. */
class PatientMapper : public Mapper<PtrPatientDO>
{
public:
    PtrPatientDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<PatientDO>();
        data->setId(resultSet->getString("id"));
        if (!resultSet->isNull("tenant_id")) data->setTenantId(resultSet->getString("tenant_id"));
        data->setCreatedAt(resultSet->getString("created_at"));
        if (!resultSet->isNull("updated_at")) data->setUpdatedAt(resultSet->getString("updated_at"));
        if (!resultSet->isNull("created_by")) data->setCreatedBy(resultSet->getString("created_by"));
        if (!resultSet->isNull("updated_by")) data->setUpdatedBy(resultSet->getString("updated_by"));
        data->setIsDeleted(resultSet->getInt("is_deleted"));
        if (!resultSet->isNull("deleted_at")) data->setDeletedAt(resultSet->getString("deleted_at"));
        if (!resultSet->isNull("deleted_by")) data->setDeletedBy(resultSet->getString("deleted_by"));
        data->setVersion(resultSet->getInt("version"));
        data->setBusNo(resultSet->getString("bus_no"));
        data->setName(resultSet->getString("name"));
        if (!resultSet->isNull("name_json")) data->setNameJson(resultSet->getString("name_json"));
        data->setGender(resultSet->getString("gender_enum"));
        if (!resultSet->isNull("birth_date")) data->setBirthDate(resultSet->getString("birth_date"));
        data->setStatusEnum(resultSet->getString("status_enum"));
        data->setActiveFlag(resultSet->getInt("active_flag"));
        data->setTempFlag(resultSet->getInt("temp_flag"));
        if (!resultSet->isNull("id_card")) data->setIdCard(resultSet->getString("id_card"));
        if (!resultSet->isNull("marital_status_enum")) data->setMaritalStatusEnum(resultSet->getString("marital_status_enum"));
        if (!resultSet->isNull("prfs_enum")) data->setPrfsEnum(resultSet->getString("prfs_enum"));
        if (!resultSet->isNull("phone")) data->setPhone(resultSet->getString("phone"));
        if (!resultSet->isNull("address")) data->setAddress(resultSet->getString("address"));
        if (!resultSet->isNull("address_province")) data->setAddressProvince(resultSet->getString("address_province"));
        if (!resultSet->isNull("address_city")) data->setAddressCity(resultSet->getString("address_city"));
        if (!resultSet->isNull("address_district")) data->setAddressDistrict(resultSet->getString("address_district"));
        if (!resultSet->isNull("address_street")) data->setAddressStreet(resultSet->getString("address_street"));
        if (!resultSet->isNull("address_extend_json")) data->setAddressExtendJson(resultSet->getString("address_extend_json"));
        if (!resultSet->isNull("nationality_code")) data->setNationalityCode(resultSet->getString("nationality_code"));
        if (!resultSet->isNull("py_str")) data->setPyStr(resultSet->getString("py_str"));
        if (!resultSet->isNull("wb_str")) data->setWbStr(resultSet->getString("wb_str"));
        if (!resultSet->isNull("blood_type_abo")) data->setBloodTypeAbo(resultSet->getInt("blood_type_abo"));
        if (!resultSet->isNull("blood_type_rh")) data->setBloodTypeRh(resultSet->getInt("blood_type_rh"));
        if (!resultSet->isNull("work_company")) data->setWorkCompany(resultSet->getString("work_company"));
        if (!resultSet->isNull("native_place")) data->setNativePlace(resultSet->getString("native_place"));
        if (!resultSet->isNull("country_code")) data->setCountryCode(resultSet->getString("country_code"));
        if (!resultSet->isNull("link_name")) data->setLinkName(resultSet->getString("link_name"));
        if (!resultSet->isNull("link_relation_code")) data->setLinkRelationCode(resultSet->getInt("link_relation_code"));
        if (!resultSet->isNull("link_telcom")) data->setLinkTelcom(resultSet->getString("link_telcom"));
        if (!resultSet->isNull("link_jsons")) data->setLinkJsons(resultSet->getString("link_jsons"));
        if (!resultSet->isNull("deceased_date")) data->setDeceasedDate(resultSet->getString("deceased_date"));
        if (!resultSet->isNull("deceased_reason")) data->setDeceasedReason(resultSet->getString("deceased_reason"));
        data->setOrganizationId(resultSet->getString("organization_id"));
        if (!resultSet->isNull("merged_target_patient_id")) data->setMergedTargetPatientId(resultSet->getString("merged_target_patient_id"));
        if (!resultSet->isNull("merged_at")) data->setMergedAt(resultSet->getString("merged_at"));
        return data;
    }
};

class PatientStringMapper : public Mapper<std::string>
{
public:
    explicit PatientStringMapper(const std::string& column) : column(column) {}

    std::string mapper(ResultSet* resultSet) const override
    {
        return resultSet->isNull(column) ? "" : resultSet->getString(column);
    }

private:
    std::string column;
};

#endif // !_PATIENT_MAPPER_H_
