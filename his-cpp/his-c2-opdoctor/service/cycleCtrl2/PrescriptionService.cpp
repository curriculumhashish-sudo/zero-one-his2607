#include "stdafx.h"
#include "PrescriptionService.h"
#include "BaseDAO.h"
#include "Mapper.h"
#include "domain/do/MedMedicationRequestDO.h"
#include "domain/do/AdmChargeItemDO.h"
#include <ctime>

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

    class RxMapper : public Mapper<MedMedicationRequestDO>
    {
    public:
        MedMedicationRequestDO mapper(ResultSet* rs) const override
        {
            MedMedicationRequestDO d;
            d.setId(rs->getString(1));
            d.setPrescription_no(rs->getString(2));
            d.setPatient_id(rs->getString(3));
            d.setEncounter_id(rs->getString(4));
            d.setStatus_enum(rs->getString(5));
            d.setCategory_enum(rs->getString(6));
            d.setCreated_at(rs->getString(7));
            d.setRequester_id(rs->getString(8));
            d.setContent_json(rs->getString(9));
            d.setRemark(rs->getString(10));
            d.setRx_type_code(rs->getString(11));
            d.setReq_authored_time(rs->getString(12));
            return d;
        }
    };

    class ChargeItemMapper : public Mapper<AdmChargeItemDO>
    {
    public:
        AdmChargeItemDO mapper(ResultSet* rs) const override
        {
            AdmChargeItemDO d;
            d.setId(rs->getString(1));
            d.setBus_no(rs->getString(2));
            d.setDefinition_id(rs->getString(3));
            d.setQuantity_value(rs->getDouble(4));
            d.setTotal_price(rs->getDouble(5));
            d.setStatus_enum(rs->getString(6));
            d.setRefund_id(rs->getString(7));
            return d;
        }
    };
}

static int calcAge(const std::string& birthDate)
{
    if (birthDate.empty()) return 0;
    int year = std::stoi(birthDate.substr(0, 4));
    time_t now = time(nullptr);
    struct tm* tm_now = gmtime(&now);
    return (tm_now->tm_year + 1900) - year;
}

static std::string rxStatusName(const std::string& statusEnum)
{
    if (statusEnum == "COMPLETED") return "completed";
    if (statusEnum == "ACTIVE")    return "active";
    if (statusEnum == "CANCELLED") return "cancelled";
    if (statusEnum == "DRAFT")     return "draft";
    return statusEnum;
}

oatpp::List<oatpp::Object<Prescription2DetailDTO>> PrescriptionService::queryPrescriptions(const Prescription2Query::Wrapper& query)
{
    auto resultList = oatpp::List<oatpp::Object<Prescription2DetailDTO>>::createShared();
    std::string encounterId = query->encounterId.getValue("");
    std::string patientNo = query->num.getValue("");
    bool today = (query->type.getValue("today") == "today");

    BaseDAO dao;
    auto* ss = dao.getSqlSession();
    std::string pid;

    if (!encounterId.empty())
    {
        pid = ss->executeQueryOne<ScalarStr>(
            "SELECT patient_id FROM adm_encounter WHERE id = ? AND is_deleted = 0",
            ScalarStrMapper(), "%s", encounterId).val;
        if (pid.empty()) return resultList;
    }
    else if (!patientNo.empty())
    {
        pid = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_patient WHERE bus_no = ? AND is_deleted = 0 LIMIT 1",
            ScalarStrMapper(), "%s", patientNo).val;
        if (pid.empty()) return resultList;
    }
    else
    {
        return resultList;
    }

    std::string patientName = ss->executeQueryOne<ScalarStr>(
        "SELECT name FROM adm_patient WHERE id = ?", ScalarStrMapper(), "%s", pid).val;
    std::string gender = ss->executeQueryOne<ScalarStr>(
        "SELECT gender_enum FROM adm_patient WHERE id = ?", ScalarStrMapper(), "%s", pid).val;
    std::string birthDate = ss->executeQueryOne<ScalarStr>(
        "SELECT birth_date FROM adm_patient WHERE id = ?", ScalarStrMapper(), "%s", pid).val;

    std::string sql =
        "SELECT id, prescription_no, patient_id, encounter_id, "
        "status_enum, category_enum, created_at, requester_id, "
        "content_json, remark, rx_type_code, req_authored_time "
        "FROM med_medication_request "
        "WHERE is_deleted = 0 ";

    if (!encounterId.empty() && today)
    {
        sql += "AND encounter_id = ? ";
    }
    else if (!pid.empty())
    {
        sql += "AND patient_id = ? ";
        if (today)
            sql += "AND DATE(created_at) = CURDATE() ";
        else
            sql += "AND DATE(created_at) < CURDATE() ";
    }

    sql += "ORDER BY created_at DESC ";

    int pageSize = query->pageSize.getValue(0);
    if (!today && pageSize > 0)
    {
        int page = query->page.getValue(1);
        if (page < 1) page = 1;
        int offset = (page - 1) * pageSize;
        sql += "LIMIT " + std::to_string(pageSize);
        if (offset > 0)
            sql += " OFFSET " + std::to_string(offset);
    }

    std::string param;
    if (!encounterId.empty() && today)
        param = encounterId;
    else
        param = pid;

    auto prescriptions = ss->executeQuery<MedMedicationRequestDO>(sql, RxMapper(), "%s", param);

    for (auto& rx : prescriptions)
    {
        auto dto = Prescription2DetailDTO::createShared();

        dto->patientName = patientName;
        dto->gender = gender;
        dto->age = calcAge(birthDate);
        dto->patientId = rx.getPatient_id();

        dto->rxId = rx.getId();
        dto->encounterId = rx.getEncounter_id();
        dto->prescriptionNo = rx.getPrescription_no();
        dto->costType = rx.getCategory_enum();
        dto->date = rx.getCreated_at();
        dto->issuedTime = rx.getReq_authored_time();
        dto->status = rx.getStatus_enum();
        dto->statusName = rxStatusName(rx.getStatus_enum());
        dto->rxType = rx.getRx_type_code();
        dto->items = rx.getContent_json();

        std::string docId = rx.getRequester_id();
        std::string docName = ss->executeQueryOne<ScalarStr>(
            "SELECT name FROM adm_staff WHERE id = ?",
            ScalarStrMapper(), "%s", docId).val;
        dto->doctor = docName.empty() ? docId : docName;

        std::string orgId = ss->executeQueryOne<ScalarStr>(
            "SELECT organization_id FROM adm_encounter "
            "WHERE id = ? AND is_deleted = 0",
            ScalarStrMapper(), "%s", rx.getEncounter_id()).val;
        std::string deptName = ss->executeQueryOne<ScalarStr>(
            "SELECT name FROM adm_organization WHERE id = ?",
            ScalarStrMapper(), "%s", orgId).val;
        dto->department = deptName.empty() ? orgId : deptName;

        dto->diagnosis = ss->executeQueryOne<ScalarStr>(
            "SELECT description FROM adm_encounter_diagnosis "
            "WHERE encounter_id = ? AND is_deleted = 0 "
            "ORDER BY created_at DESC LIMIT 1",
            ScalarStrMapper(), "%s", rx.getEncounter_id()).val;

        auto chargeItems = ss->executeQuery<AdmChargeItemDO>(
            "SELECT id, bus_no, definition_id, quantity_value, total_price, status_enum, refund_id "
            "FROM adm_charge_item "
            "WHERE prescription_no = ? AND is_deleted = 0",
            ChargeItemMapper(), "%s", rx.getPrescription_no());
        double tp = 0.0;
        for (auto& ci : chargeItems) tp += ci.getTotal_price();
        dto->totalPrice = tp;

        resultList->push_back(dto);
    }
    return resultList;
}
