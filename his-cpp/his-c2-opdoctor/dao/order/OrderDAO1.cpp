#include "stdafx.h"
#include "OrderDAO1.h"
#include "mapper/MedicationDefinitionMapper1.h"
#include "mapper/ActivityDefinitionMapper1.h"
#include "mapper/DeviceDefinitionMapper1.h"
#include "mapper/MedicationRequestMapper1.h"
#include "mapper/ServiceRequestMapper1.h"
#include "mapper/DeviceRequestMapper1.h"
#include "mapper/ChargeItemDefinitionMapper1.h"
#include "mapper/PractitionerMapper1.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>
#include <iomanip>
#include <ctime>

std::string OrderDAO1::getMedicationIdByBusNo(const std::string& busNo)
{
    if (busNo.empty()) return "";
    std::string sql = "SELECT * FROM med_medication_definition WHERE bus_no = ? AND status_enum = 'ACTIVE' AND is_deleted = 0 LIMIT 1";
    auto result = sqlSession->executeQueryOne<PtrMedicationDefinitionDO1>(sql, PtrMedicationDefinitionMapper1(), "%s", busNo);
    if (result) {
        return result->getId();
    }
    return "";
}

std::string OrderDAO1::getActivityIdByBusNo(const std::string& busNo)
{
    if (busNo.empty()) return "";
    std::string sql = "SELECT * FROM wkf_activity_definition WHERE bus_no = ? AND status_enum = 'ACTIVE' AND is_deleted = 0 LIMIT 1";
    auto result = sqlSession->executeQueryOne<PtrActivityDefinitionDO1>(sql, PtrActivityDefinitionMapper1(), "%s", busNo);
    if (result) {
        return result->getId();
    }
    return "";
}

std::string OrderDAO1::getDeviceDefIdByBusNo(const std::string& busNo)
{
    if (busNo.empty()) return "";
    std::string sql = "SELECT * FROM adm_device_definition WHERE bus_no = ? AND status_enum = 'ACTIVE' AND is_deleted = 0 LIMIT 1";
    auto result = sqlSession->executeQueryOne<PtrDeviceDefinitionDO1>(sql, PtrDeviceDefinitionMapper1(), "%s", busNo);
    if (result) {
        return result->getId();
    }
    return "";
}

uint64_t OrderDAO1::getUnitPrice(const std::string& instanceTable, const std::string& instanceId)
{
    if (instanceTable.empty() || instanceId.empty()) {
        return 0.0;
    }
    std::string sql = "SELECT price_value FROM adm_charge_item_definition "
        "WHERE instance_table = ? AND instance_id = ? AND status_enum = 'ACTIVE' "
        "ORDER BY priority ASC, created_at DESC LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, instanceTable);
    SQLPARAMS_PUSH(params, "s", std::string, instanceId);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::string OrderDAO1::getPractitionerName(const std::string& practitionerId)
{
    if (practitionerId.empty()) return "";
    std::string sql = "SELECT name FROM adm_practitioner WHERE id = ? AND status_enum = 'ACTIVE'";
    auto result = sqlSession->executeQueryOne<PtrPractitionerDO1>(sql, PtrPractitionerMapper1(), "%s", practitionerId);
    if (result) {
        return result->getName();
    }
    return "";
}

std::list<OrderDetailDTO1::Wrapper> OrderDAO1::queryOrderList(const std::string& encounterId, const std::string& patientId,
    uint64_t pageIndex, uint64_t pageSize, uint64_t& total)
{
    std::list<OrderDetailDTO1::Wrapper> resultList;

    // 分页参数校验
    if (pageIndex < 1) pageIndex = 1;
    if (pageSize < 1) pageSize = 10;

    // 1. 查询药品医嘱
    SqlParams medicationParams;
    std::string medSql = "SELECT * FROM med_medication_request WHERE encounter_id = ? AND patient_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(medicationParams, "s", std::string, encounterId);
    SQLPARAMS_PUSH(medicationParams, "s", std::string, patientId);

    auto medOrders = sqlSession->executeQuery<MedicationRequestDO1>(medSql, MedicationRequestMapper1(), medicationParams);

    // 2. 查询诊疗医嘱
    SqlParams serviceParams;
    std::string svcSql = "SELECT * FROM wkf_service_request WHERE encounter_id = ? AND patient_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(serviceParams, "s", std::string, encounterId);
    SQLPARAMS_PUSH(serviceParams, "s", std::string, patientId);

    auto svcOrders = sqlSession->executeQuery<ServiceRequestDO1>(svcSql, ServiceRequestMapper1(), serviceParams);

    // 3. 查询耗材医嘱
    SqlParams deviceParams;
    std::string devSql = "SELECT * FROM wkf_device_request WHERE encounter_id = ? AND patient_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(deviceParams, "s", std::string, encounterId);
    SQLPARAMS_PUSH(deviceParams, "s", std::string, patientId);

    auto devOrders = sqlSession->executeQuery<DeviceRequestDO1>(devSql, DeviceRequestMapper1(), deviceParams);

    // 4. 合并到临时列表
    std::list<std::pair<std::string, OrderDetailDTO1::Wrapper>> mergedList;

    // 处理药品医嘱
    for (auto& item : medOrders) {
        auto dto = OrderDetailDTO1::createShared();
        dto->orderId = oatpp::String(item.getId());
        dto->visitId = oatpp::String(item.getEncounterId());
        dto->patientId = oatpp::String(item.getPatientId());
        dto->doctorId = oatpp::String(item.getRequesterId());
        dto->doctorName = oatpp::String(getPractitionerName(item.getRequesterId()));

        // 获取药品名称和规格
        std::string medSql2 = "SELECT * FROM med_medication_definition WHERE id = ? AND status_enum = 'ACTIVE'";
        auto medDef = sqlSession->executeQueryOne<PtrMedicationDefinitionDO1>(medSql2, PtrMedicationDefinitionMapper1(), "%s", item.getMedicationId());
        if (medDef) {
            dto->itemName = oatpp::String(medDef->getName());
            dto->spec = oatpp::String(medDef->getTotalVolume());
        }

        dto->itemCode = oatpp::String(item.getBusNo());
        dto->orderType = oatpp::String("medication");
        dto->dosage = oatpp::String(std::to_string(item.getDose()));
        dto->frequency = oatpp::String(item.getRateCode());
        dto->routeCode = oatpp::String(item.getMethodCode());

        // ========== 第一处：计算药品使用天数 ==========
        int days = 0;
        if (!item.getEffectiveDoseStart().empty() && !item.getEffectiveDoseEnd().empty()) {
            std::tm tmStart = {}, tmEnd = {};
            std::istringstream ssStart(item.getEffectiveDoseStart());
            std::istringstream ssEnd(item.getEffectiveDoseEnd());
            ssStart >> std::get_time(&tmStart, "%Y-%m-%d %H:%M:%S");
            ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d %H:%M:%S");
            if (!ssStart.fail() && !ssEnd.fail()) {
                std::time_t timeStart = std::mktime(&tmStart);
                std::time_t timeEnd = std::mktime(&tmEnd);
                days = (int)((timeEnd - timeStart) / (24 * 3600));
                if (days < 0) days = 0;
            }
        }
        dto->useDays = oatpp::Int32(days);

        dto->quantity = oatpp::Int32((int)item.getQuantity());
        dto->isSkinTest = oatpp::Boolean(item.getSkinTestFlag() == 1);
        dto->status = oatpp::String(item.getStatusEnum());
        dto->createTime = oatpp::String(item.getCreatedAt());

        // 计算总价
        double unitPrice = getUnitPrice("med_medication_definition", item.getMedicationId());
        dto->unitPrice = oatpp::Float64(unitPrice);
        dto->totalPrice = oatpp::Float64(unitPrice * item.getQuantity());

        dto->groupId = oatpp::String(item.getGroupId());
        dto->parentOrderId = oatpp::String("");
        dto->remark = oatpp::String(item.getRemark());

        mergedList.push_back({ item.getCreatedAt(), dto });
    }

    // 处理诊疗医嘱
    for (auto& item : svcOrders) {
        auto dto = OrderDetailDTO1::createShared();
        dto->orderId = oatpp::String(item.getId());
        dto->visitId = oatpp::String(item.getEncounterId());
        dto->patientId = oatpp::String(item.getPatientId());
        dto->doctorId = oatpp::String(item.getRequesterId());
        dto->doctorName = oatpp::String(getPractitionerName(item.getRequesterId()));

        // 获取诊疗名称和规格
        std::string actSql = "SELECT * FROM wkf_activity_definition WHERE id = ? AND status_enum = 'ACTIVE'";
        auto actDef = sqlSession->executeQueryOne<PtrActivityDefinitionDO1>(actSql, PtrActivityDefinitionMapper1(), "%s", item.getActivityId());
        if (actDef) {
            dto->itemName = oatpp::String(actDef->getName());
            dto->spec = oatpp::String(actDef->getDescriptionText());
        }

        dto->itemCode = oatpp::String(item.getBusNo());
        dto->orderType = oatpp::String("service");
        dto->dosage = oatpp::String("");
        dto->frequency = oatpp::String(item.getRateCode());
        dto->routeCode = oatpp::String("");

        // ========== 第二处：计算诊疗使用天数 ==========
        int days = 0;
        if (!item.getOccurrenceStartTime().empty() && !item.getOccurrenceEndTime().empty()) {
            std::tm tmStart = {}, tmEnd = {};
            std::istringstream ssStart(item.getOccurrenceStartTime());
            std::istringstream ssEnd(item.getOccurrenceEndTime());
            ssStart >> std::get_time(&tmStart, "%Y-%m-%d %H:%M:%S");
            ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d %H:%M:%S");
            if (!ssStart.fail() && !ssEnd.fail()) {
                std::time_t timeStart = std::mktime(&tmStart);
                std::time_t timeEnd = std::mktime(&tmEnd);
                days = (int)((timeEnd - timeStart) / (24 * 3600));
                if (days < 0) days = 0;
            }
        }
        dto->useDays = oatpp::Int32(days);

        dto->quantity = oatpp::Int32((int)item.getQuantity());
        dto->isSkinTest = oatpp::Boolean(false);
        dto->status = oatpp::String(item.getStatusEnum());
        dto->createTime = oatpp::String(item.getCreatedAt());

        // 计算总价
        double unitPrice = getUnitPrice("wkf_activity_definition", item.getActivityId());
        dto->unitPrice = oatpp::Float64(unitPrice);
        dto->totalPrice = oatpp::Float64(unitPrice * item.getQuantity());

        dto->groupId = oatpp::String(item.getGroupId());
        dto->parentOrderId = oatpp::String(item.getParentId());
        dto->remark = oatpp::String(item.getRemark());

        mergedList.push_back({ item.getCreatedAt(), dto });
    }

    // 处理耗材医嘱
    for (auto& item : devOrders) {
        auto dto = OrderDetailDTO1::createShared();
        dto->orderId = oatpp::String(item.getId());
        dto->visitId = oatpp::String(item.getEncounterId());
        dto->patientId = oatpp::String(item.getPatientId());
        dto->doctorId = oatpp::String(item.getRequesterId());
        dto->doctorName = oatpp::String(getPractitionerName(item.getRequesterId()));

        // 获取耗材名称和规格
        std::string devSql2 = "SELECT * FROM adm_device_definition WHERE id = ? AND status_enum = 'ACTIVE'";
        auto devDef = sqlSession->executeQueryOne<PtrDeviceDefinitionDO1>(devSql2, PtrDeviceDefinitionMapper1(), "%s", item.getDeviceDefId());
        if (devDef) {
            dto->itemName = oatpp::String(devDef->getName());
            dto->spec = oatpp::String(devDef->getTotalVolume());
        }

        dto->itemCode = oatpp::String(item.getBusNo());
        dto->orderType = oatpp::String("device");
        dto->dosage = oatpp::String("");
        dto->frequency = oatpp::String(item.getRateCode());
        dto->routeCode = oatpp::String("");

        // ========== 第三处：计算耗材使用天数 ==========
        int days = 0;
        if (!item.getUseStartTime().empty() && !item.getUseEndTime().empty()) {
            std::tm tmStart = {}, tmEnd = {};
            std::istringstream ssStart(item.getUseStartTime());
            std::istringstream ssEnd(item.getUseEndTime());
            ssStart >> std::get_time(&tmStart, "%Y-%m-%d %H:%M:%S");
            ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d %H:%M:%S");
            if (!ssStart.fail() && !ssEnd.fail()) {
                std::time_t timeStart = std::mktime(&tmStart);
                std::time_t timeEnd = std::mktime(&tmEnd);
                days = (int)((timeEnd - timeStart) / (24 * 3600));
                if (days < 0) days = 0;
            }
        }
        dto->useDays = oatpp::Int32(days);

        dto->quantity = oatpp::Int32((int)item.getQuantity());
        dto->isSkinTest = oatpp::Boolean(false);
        dto->status = oatpp::String(item.getStatusEnum());
        dto->createTime = oatpp::String(item.getCreatedAt());

        // 计算总价
        double unitPrice = getUnitPrice("adm_device_definition", item.getDeviceDefId());
        dto->unitPrice = oatpp::Float64(unitPrice);
        dto->totalPrice = oatpp::Float64(unitPrice * item.getQuantity());

        dto->groupId = oatpp::String("");
        dto->parentOrderId = oatpp::String(item.getBasedOnId());
        dto->remark = oatpp::String(item.getRemark());

        mergedList.push_back({ item.getCreatedAt(), dto });
    }

    // 按创建时间降序排序
    mergedList.sort([](const auto& a, const auto& b) {
        return a.first > b.first;
        });

    // 计算总条数
    total = mergedList.size();

    // 分页截取
    uint64_t start = (pageIndex - 1) * pageSize;
    uint64_t end = std::min(start + pageSize, total);

    uint64_t idx = 0;
    for (auto it = mergedList.begin(); it != mergedList.end() && idx < end; ++it, ++idx) {
        if (idx >= start) {
            resultList.push_back(it->second);
        }
    }

    return resultList;
}

bool OrderDAO1::insertMedicationOrder(const MedicationRequestDO1& data)
{
    return insert(data) == 1;
}

bool OrderDAO1::insertServiceOrder(const ServiceRequestDO1& data)
{
    return insert(data) == 1;
}

bool OrderDAO1::insertDeviceOrder(const DeviceRequestDO1& data)
{
    return insert(data) == 1;
}

bool OrderDAO1::updateMedicationOrder(const MedicationRequestDO1& data)
{
    return update(data) == 1;
}

bool OrderDAO1::updateServiceOrder(const ServiceRequestDO1& data)
{
    return update(data) == 1;
}

bool OrderDAO1::updateDeviceOrder(const DeviceRequestDO1& data)
{
    return update(data) == 1;
}

PtrMedicationRequestDO1 OrderDAO1::selectMedicationOrderById(const std::string& id)
{
    std::string sql = "SELECT * FROM med_medication_request WHERE id = ? AND is_deleted = 0";
    return sqlSession->executeQueryOne<PtrMedicationRequestDO1>(sql, PtrMedicationRequestMapper1(), "%s", id);
}

PtrServiceRequestDO1 OrderDAO1::selectServiceOrderById(const std::string& id)
{
    std::string sql = "SELECT * FROM wkf_service_request WHERE id = ? AND is_deleted = 0";
    return sqlSession->executeQueryOne<PtrServiceRequestDO1>(sql, PtrServiceRequestMapper1(), "%s", id);
}

PtrDeviceRequestDO1 OrderDAO1::selectDeviceOrderById(const std::string& id)
{
    std::string sql = "SELECT * FROM wkf_device_request WHERE id = ? AND is_deleted = 0";
    return sqlSession->executeQueryOne<PtrDeviceRequestDO1>(sql, PtrDeviceRequestMapper1(), "%s", id);
}