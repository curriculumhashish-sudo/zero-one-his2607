#include "stdafx.h"
#include "AdmissionService.h"
#include "BaseDAO.h"
#include "Mapper.h"
#include "domain/do/AdmEncounterDO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

namespace {
    struct ScalarStr { std::string val; };
    class ScalarStrMapper : public Mapper<ScalarStr>
    {
    public:
        ScalarStr mapper(ResultSet* rs) const override
        {
            ScalarStr s; s.val = rs->getString(1); return s;
        }
    };
}

oatpp::Object<Admission2ResultDTO> AdmissionService::applyAdmission(const RequestInpatientAdmissionDTO::Wrapper& dto,
    const std::string& operatorId)
{
    auto result = Admission2ResultDTO::createShared();
    std::string encounterId = dto->encounterId.getValue("");
    std::string patientId = dto->patientId.getValue("");
    std::string deptId = dto->targetDepartmentId.getValue("");
    std::string wardId = dto->wardId.getValue("");

    BaseDAO dao;
    auto* ss = dao.getSqlSession();

    std::string encId = ss->executeQueryOne<ScalarStr>(
        "SELECT id FROM adm_encounter "
        "WHERE id = ? AND patient_id = ? AND class_enum = 'AMB' "
        "AND status_enum = 'arrived' AND is_deleted = 0 LIMIT 1",
        ScalarStrMapper(), "%s%s", encounterId, patientId).val;

    if (encId.empty())
    {
        result->status = "FAILED";
        result->statusName = "no valid outpatient encounter found";
        return result;
    }

    std::string existingImp = ss->executeQueryOne<ScalarStr>(
        "SELECT id FROM adm_encounter "
        "WHERE amb_encounter_id = ? AND class_enum = 'IMP' AND status_enum IN ('planned','arrived') "
        "AND is_deleted = 0 LIMIT 1",
        ScalarStrMapper(), "%s", encId).val;
    if (!existingImp.empty())
    {
        std::string impBusNo = ss->executeQueryOne<ScalarStr>(
            "SELECT bus_no FROM adm_encounter WHERE id = ?",
            ScalarStrMapper(), "%s", existingImp).val;
        std::string impStatus = ss->executeQueryOne<ScalarStr>(
            "SELECT status_enum FROM adm_encounter WHERE id = ?",
            ScalarStrMapper(), "%s", existingImp).val;
        result->applicationId = existingImp;
        result->applicationNo = impBusNo;
        result->inpatientEncounterId = existingImp;
        result->inpatientEncounterNo = impBusNo;
        result->status = "DUPLICATE";
        result->statusName = "admission already exists";
        return result;
    }

    std::string orgId = wardId.empty() ? deptId : wardId;
    std::string now = SimpleDateTimeFormat::format();
    UuidFacade uf;
    std::string newId = uf.genUuid();

    AdmEncounterDO enc;
    enc.setId(newId);
    enc.setPatient_id(patientId);
    enc.setClass_enum("IMP");
    enc.setStatus_enum("planned");
    enc.setOrganization_id(orgId);
    enc.setAdmit_source_code(dto->admissionTypeCode.getValue(""));
    enc.setIn_way_code(dto->admissionWayCode.getValue(""));
    enc.setPriority_enum(dto->priorityCode.getValue(""));
    enc.setStart_time(now);
    enc.setCreated_by(operatorId);
    enc.setAmb_encounter_id(encId);

    if (dao.insert(enc) != 1)
    {
        result->status = "FAILED";
        result->statusName = "create encounter failed";
        return result;
    }

    auto& diagnoses = *dto->admissionDiagnoses;
    for (auto& diag : diagnoses)
    {
        std::string diagName = diag->name.getValue("");
        std::string diagCode = diag->code.getValue("");
        std::string diagDesc = diag->description.getValue("");
        std::string catalogId = diag->catalogId.getValue("");
        bool primary = diag->primary.getValue(false);

        if (diagName.empty()) continue;

        if (diagDesc.empty()) diagDesc = diagName;

        std::string diagId = uf.genUuid();
        std::string sql =
            "INSERT INTO adm_encounter_diagnosis "
            "(id, tenant_id, created_at, updated_at, created_by, updated_by, is_deleted, version, "
            "encounter_id, condition_id, primary_flag, `rank`, description, "
            "diag_type_code, condition_code, tcm_syndrome_group, tcm_flag) "
            "VALUES (?, '1', ?, ?, ?, 1, 0, 0, ?, ?, ?, 1, ?, 'WESTERN', ?, '', 0)";
        ss->executeUpdate(sql, "%s%s%s%s%s%s%s%i%s",
            diagId, now, now, operatorId, newId, catalogId,
            primary ? 1 : 0, diagDesc, diagCode);
    }

    std::string busNo = ss->executeQueryOne<ScalarStr>(
        "SELECT bus_no FROM adm_encounter WHERE id = ?", ScalarStrMapper(), "%s", newId).val;

    result->applicationId = newId;
    result->applicationNo = busNo;
    result->inpatientEncounterId = newId;
    result->inpatientEncounterNo = busNo;
    result->status = "SUCCESS";
    result->statusName = "admission applied";
    result->createdAt = now;

    return result;
}
