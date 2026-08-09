#include "stdafx.h"
#include "CheckInpatientAdmissionService.h"
#include "BaseDAO.h"
#include "Mapper.h"

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

oatpp::Object<CheckInpatientAdmissionVO> CheckInpatientAdmissionService::check(const CheckInpatientAdmissionDTO::Wrapper& dto)
{
    std::string encounterId = dto->encounterId.getValue("");
    if (encounterId.empty()) return nullptr;

    BaseDAO dao;
    auto* ss = dao.getSqlSession();

    std::string encClass = ss->executeQueryOne<ScalarStr>(
        "SELECT class_enum FROM adm_encounter WHERE id = ? AND is_deleted = 0",
        ScalarStrMapper(), "%s", encounterId).val;
    if (encClass.empty()) return nullptr;

    auto result = CheckInpatientAdmissionVO::createShared();
    result->exists = false;

    std::string impId = ss->executeQueryOne<ScalarStr>(
        "SELECT id FROM adm_encounter "
        "WHERE amb_encounter_id = ? AND class_enum = 'IMP' "
        "AND status_enum IN ('planned','arrived') AND is_deleted = 0 "
        "ORDER BY start_time DESC LIMIT 1",
        ScalarStrMapper(), "%s", encounterId).val;

    if (impId.empty()) return result;

    std::string impStatus = ss->executeQueryOne<ScalarStr>(
        "SELECT status_enum FROM adm_encounter WHERE id = ?",
        ScalarStrMapper(), "%s", impId).val;
    std::string impNo = ss->executeQueryOne<ScalarStr>(
        "SELECT bus_no FROM adm_encounter WHERE id = ?",
        ScalarStrMapper(), "%s", impId).val;

    result->exists = true;
    result->applicationId = impId;
    result->inpatientEncounterId = impId;
    result->inpatientEncounterNo = impNo;
    result->status = impStatus == "planned" ? "PLANNED" : "ADMITTED";
    result->statusName = impStatus == "planned" ? "planned" : "admitted";

    return result;
}
