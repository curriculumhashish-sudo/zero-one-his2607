/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2022/12/03 11:01:02
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
      https://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "RefundService.h"
#include "BaseDAO.h"
#include "Mapper.h"
#include "domain/do/AdmChargeItemDO.h"
#include "domain/do/AdmInvoiceDO.h"
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

    struct ScalarDouble { double val; };
    class ScalarDoubleMapper : public Mapper<ScalarDouble>
    {
    public:
        ScalarDouble mapper(ResultSet* rs) const override
        {
            ScalarDouble s; s.val = rs->getDouble(1); return s;
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
            d.setQuantity_unit(rs->getString(8));
            d.setUnit_price(rs->getDouble(9));
            d.setOccurrence_time(rs->getString(10));
            d.setService_table(rs->getString(11));
            d.setEncounter_id(rs->getString(12));
            d.setPatient_id(rs->getString(13));
            return d;
        }
    };
}

static std::string statusDisplayName(const std::string& statusEnum)
{
    if (statusEnum == "SETTLED") return "settled";
    if (statusEnum == "DRAFT")   return "draft";
    if (statusEnum == "CANCELLED") return "cancelled";
    if (statusEnum == "refunded") return "refunded";
    return statusEnum;
}

oatpp::List<oatpp::Object<Refund2DetailDTO>> RefundService::queryRefundList(const Refund2Query::Wrapper& query)
{
    auto resultList = oatpp::List<oatpp::Object<Refund2DetailDTO>>::createShared();
    std::string encounterId = query->encounterId.getValue("");

    BaseDAO dao;
    auto* ss = dao.getSqlSession();

    if (encounterId.empty())
    {
        std::string patientNo = query->num.getValue("");
        if (patientNo.empty()) return resultList;

        std::string pid = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_patient WHERE bus_no = ? AND is_deleted = 0 LIMIT 1",
            ScalarStrMapper(), "%s", patientNo).val;
        if (pid.empty()) return resultList;

        encounterId = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_encounter "
            "WHERE patient_id = ? AND is_deleted = 0 "
            "ORDER BY start_time DESC LIMIT 1",
            ScalarStrMapper(), "%s", pid).val;
        if (encounterId.empty()) return resultList;
    }

    std::string pid = ss->executeQueryOne<ScalarStr>(
        "SELECT patient_id FROM adm_encounter WHERE id = ? AND is_deleted = 0",
        ScalarStrMapper(), "%s", encounterId).val;
    if (pid.empty()) return resultList;

    std::string patientName = ss->executeQueryOne<ScalarStr>(
        "SELECT name FROM adm_patient WHERE id = ?", ScalarStrMapper(), "%s", pid).val;

    auto items = ss->executeQuery<AdmChargeItemDO>(
        "SELECT id, bus_no, definition_id, quantity_value, total_price, status_enum, refund_id, "
        "quantity_unit, unit_price, occurrence_time, service_table, encounter_id, patient_id "
        "FROM adm_charge_item "
        "WHERE encounter_id = ? AND is_deleted = 0 "
        "ORDER BY created_at DESC",
        ChargeItemMapper(), "%s", encounterId);

    for (auto& item : items)
    {
        auto dto = Refund2DetailDTO::createShared();
        dto->patientName = patientName;
        dto->paymentNo = item.getId();
        dto->itemName = item.getDefinition_id();
        dto->itemCode = item.getDefinition_id();
        dto->itemType = item.getService_table();
        dto->quantity = item.getQuantity_value();
        dto->quantityUnit = item.getQuantity_unit();
        dto->amount = item.getTotal_price();
        dto->chargeTime = item.getOccurrence_time();
        dto->chargeStatus = item.getStatus_enum();
        dto->chargeStatusName = statusDisplayName(item.getStatus_enum());

        bool refunded = !item.getRefund_id().empty();
        dto->refundStatus = refunded ? "refunded" : "";
        dto->refundStatusName = refunded ? "refunded" : "";

        if (!refunded && item.getStatus_enum() == "SETTLED")
        {
            dto->refundableQuantity = item.getQuantity_value();
            dto->refundableAmount = item.getTotal_price();
            dto->refundedAmount = 0.0;
            dto->canRefund = "true";
            dto->cannotRefundReason = "";
        }
        else
        {
            dto->refundableQuantity = 0.0;
            dto->refundableAmount = 0.0;
            dto->refundedAmount = item.getTotal_price();
            dto->canRefund = "false";
            if (refunded)
                dto->cannotRefundReason = "already refunded";
            else if (item.getStatus_enum() == "DRAFT")
                dto->cannotRefundReason = "not charged";
            else
                dto->cannotRefundReason = "status not refundable: " + item.getStatus_enum();
        }

        resultList->push_back(dto);
    }
    return resultList;
}

oatpp::Object<Refund2ApplyResultDTO> RefundService::applyRefund(const Refund2ApplyDTO::Wrapper& dto, const std::string& operatorId)
{
    auto result = Refund2ApplyResultDTO::createShared();
    std::string encounterId = dto->encounterId.getValue("");

    BaseDAO dao;
    auto* ss = dao.getSqlSession();

    if (encounterId.empty())
    {
        std::string patientNo = dto->patientNo.getValue("");
        if (patientNo.empty())
        {
            result->status = "FAILED";
            result->statusName = "encounterId or patientNo is required";
            return result;
        }
        std::string pid = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_patient WHERE bus_no = ? AND is_deleted = 0 LIMIT 1",
            ScalarStrMapper(), "%s", patientNo).val;
        if (pid.empty())
        {
            result->status = "FAILED";
            result->statusName = "patient not found";
            return result;
        }
        encounterId = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_encounter "
            "WHERE patient_id = ? AND class_enum = 'AMB' AND status_enum = 'arrived' "
            "AND is_deleted = 0 ORDER BY start_time DESC LIMIT 1",
            ScalarStrMapper(), "%s", pid).val;
        if (encounterId.empty())
        {
            result->status = "FAILED";
            result->statusName = "no valid outpatient encounter found";
            return result;
        }
    }

    std::string pid = ss->executeQueryOne<ScalarStr>(
        "SELECT patient_id FROM adm_encounter WHERE id = ? AND is_deleted = 0",
        ScalarStrMapper(), "%s", encounterId).val;
    if (pid.empty())
    {
        result->status = "FAILED";
        result->statusName = "encounter not found";
        return result;
    }

    if (dto->idempotencyKey && !dto->idempotencyKey.getValue("").empty())
    {
        std::string existId = ss->executeQueryOne<ScalarStr>(
            "SELECT id FROM adm_invoice WHERE bus_no = ? AND status_enum = 'refund' AND is_deleted = 0 LIMIT 1",
            ScalarStrMapper(), "%s", dto->idempotencyKey.getValue("")).val;
        if (!existId.empty())
        {
            result->applicationId = existId;
            result->applicationNo = dto->idempotencyKey.getValue("");
            result->status = "DUPLICATE";
            result->statusName = "duplicate request";
            result->refundAmount = 0.0;
            return result;
        }
    }

    std::list<std::string> itemIds;
    double totalRefundAmount = 0.0;

    auto& items = *dto->apply;
    for (auto& applyItem : items)
    {
        std::string chargeId = applyItem->paymentNo.getValue("");
        if (chargeId.empty()) continue;

        auto ci = ss->executeQueryOne<AdmChargeItemDO>(
            "SELECT id, bus_no, definition_id, quantity_value, total_price, status_enum, refund_id, "
            "quantity_unit, unit_price, occurrence_time, service_table, encounter_id, patient_id "
            "FROM adm_charge_item WHERE id = ? AND is_deleted = 0",
            ChargeItemMapper(), "%s", chargeId);
        if (ci.getId().empty())
        {
            result->status = "FAILED";
            result->statusName = "charge item not found: " + chargeId;
            return result;
        }

        if (ci.getEncounter_id() != encounterId)
        {
            result->status = "FAILED";
            result->statusName = "charge item not in this encounter: " + chargeId;
            return result;
        }

        if (ci.getStatus_enum() != "SETTLED")
        {
            result->status = "FAILED";
            result->statusName = "charge item not refundable: " + chargeId + "(" + ci.getStatus_enum() + ")";
            return result;
        }

        if (!ci.getRefund_id().empty())
        {
            result->status = "FAILED";
            result->statusName = "charge item already refunded: " + chargeId;
            return result;
        }

        itemIds.push_back(chargeId);
        totalRefundAmount += ci.getTotal_price();
    }

    if (itemIds.empty())
    {
        result->status = "FAILED";
        result->statusName = "refund item list is empty";
        return result;
    }

    UuidFacade uf;
    std::string refundId = uf.genUuid();
    std::string now = SimpleDateTimeFormat::format();
    std::string busNo = dto->idempotencyKey.getValue("");
    if (busNo.empty()) busNo = refundId;

    AdmInvoiceDO invoice;
    invoice.setId(refundId);
    invoice.setTenant_id("1");
    invoice.setCreated_at(now);
    invoice.setPatient_id(pid);
    invoice.setBus_no(busNo);
    invoice.setAmount(totalRefundAmount);
    invoice.setStatus_enum("refund");
    invoice.setIssued_at(now);
    invoice.setCreated_by(operatorId);

    if (dao.insert(invoice) != 1)
    {
        result->status = "FAILED";
        result->statusName = "create refund record failed";
        return result;
    }

    std::string sql = "UPDATE adm_charge_item SET refund_id = ?, status_enum = 'refunded', updated_at = NOW() WHERE id IN (";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, refundId);
    for (size_t i = 0; i < itemIds.size(); ++i)
    {
        if (i > 0) sql += ",";
        sql += "?";
    }
    sql += ") AND is_deleted = 0";
    for (auto& id : itemIds)
        SQLPARAMS_PUSH(params, "s", std::string, id);
    ss->executeUpdate(sql, params);

    result->applicationId = refundId;
    result->applicationNo = busNo;
    result->status = "SUCCESS";
    result->statusName = "refund applied";
    result->refundAmount = totalRefundAmount;
    result->createdAt = now;

    return result;
}
