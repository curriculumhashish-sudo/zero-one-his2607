// service/order/OrderService1.cpp
#include "stdafx.h"
#include "OrderService1.h"
#include "../../dao/order/OrderDAO1.h"
#include "../../dao/order/OrderAuditDAO1.h"
#include "../../dao/order/EnumDAO.h"
#include "../../service/session/SessionService.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "domain/do/order/MedicationRequestDO1.h"
#include "domain/do/order/ServiceRequestDO1.h"
#include "domain/do/order/DeviceRequestDO1.h"
#include "RedisClient.h"
#include "MongoClient.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// ========== 日期工具函数 ==========
static std::string addDaysToDate(const std::string& dateStr, int days)
{
    if (dateStr.empty()) {
        return SimpleDateTimeFormat::format();
    }

    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        return SimpleDateTimeFormat::format();
    }

    std::time_t time = std::mktime(&tm);
    time += days * 24 * 3600;

    std::tm* resultTm = std::localtime(&time);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", resultTm);
    return std::string(buffer);
}

// ========== 构造函数 ==========
OrderService1::OrderService1(std::shared_ptr<RedisClient> redisClient,
    std::shared_ptr<MongoClient> mongoClient)
    : m_redisClient(redisClient)
    , m_mongoClient(mongoClient)
{
    m_sessionService = std::make_shared<SessionService>(redisClient);
    m_auditDAO = std::make_shared<OrderAuditDAO1>(mongoClient);
    m_enumDAO = std::make_shared<EnumDAO>(mongoClient);
}

// ========== 枚举校验方法（带错误信息） ==========
bool OrderService1::validateEnumCode(const std::string& enumType,
    const std::string& code,
    std::string& outName,
    std::string& errorMsg) {
    if (!m_enumDAO) {
        errorMsg = "MongoDB enum service is unavailable";
        return false;
    }

    if (code.empty()) {
        errorMsg = enumType + " code cannot be empty";
        return false;
    }

    EnumValue val;
    if (m_enumDAO->getEnumValue(enumType, code, val)) {
        outName = val.name;
        return true;
    }

    errorMsg = "Invalid " + enumType + " code: " + code;
    return false;
}

bool OrderService1::validateEnumCode(const std::string& enumType,
    const std::string& code,
    std::string& errorMsg) {
    std::string dummy;
    return validateEnumCode(enumType, code, dummy, errorMsg);
}

// ========== 获取枚举值列表 ==========
std::vector<EnumValue> OrderService1::getEnumValues(const std::string& enumType) {
    if (m_enumDAO) {
        return m_enumDAO->getEnumValues(enumType);
    }
    return std::vector<EnumValue>();
}

// ========== 构建上下文 ==========
OrderService1::ServiceContext OrderService1::buildContext(const PayloadDTO* payload)
{
    ServiceContext ctx;
    ctx.valid = false;

    if (!payload) {
        return ctx;
    }

    ctx.userId = payload->getId();
    ctx.username = payload->getUsername();
    ctx.practitionerId = payload->getPractitionerId();
    ctx.practitionerName = payload->getPractitionerName();

    auto tenantIdOpt = m_sessionService->getTenantId(ctx.userId);
    ctx.tenantId = tenantIdOpt.has_value() ? tenantIdOpt.value() : "DEFAULT_TENANT";

    auto orgIdOpt = m_sessionService->getOrgId(ctx.userId);
    ctx.orgId = orgIdOpt.has_value() ? orgIdOpt.value() : "DEFAULT_ORG";

    ctx.valid = true;
    return ctx;
}

// ========== 审计日志 ==========
void OrderService1::auditLog(
    const ServiceContext& ctx,
    const std::string& orderId,
    const std::string& orderType,
    const std::string& operationType,
    const std::string& beforeData,
    const std::string& afterData,
    const std::list<std::string>& changedFields,
    const std::string& remark)
{
    if (!m_auditDAO) {
        return;
    }

    auto audit = OrderAuditDTO1::createShared();
    audit->orderId = oatpp::String(orderId);
    audit->orderType = oatpp::String(orderType);
    audit->operationType = oatpp::String(operationType);
    audit->operatorId = oatpp::String(ctx.practitionerId);
    audit->operatorName = oatpp::String(ctx.practitionerName);
    audit->operatorOrgId = oatpp::String(ctx.orgId);
    audit->operationTime = oatpp::String(SimpleDateTimeFormat::format());
    audit->remark = oatpp::String(remark);

    if (!beforeData.empty()) {
        audit->beforeData = oatpp::String(beforeData);
    }
    if (!afterData.empty()) {
        audit->afterData = oatpp::String(afterData);
    }

    if (!changedFields.empty()) {
        auto list = oatpp::List<oatpp::String>::createShared();
        for (const auto& field : changedFields) {
            list->push_back(oatpp::String(field));
        }
        audit->changedFields = list;
    }

    m_auditDAO->saveAuditLog(audit);
}

// ========== 查询医嘱列表 ==========
OrderDetailPageDTO1::Wrapper OrderService1::queryOrderList(
    const OrderQuery1::Wrapper& query,
    const PayloadDTO* payload)
{
    auto pageDto = OrderDetailPageDTO1::createShared();
    pageDto->pageIndex = query->pageIndex;
    pageDto->pageSize = query->pageSize;

    std::string doctorName = payload ? payload->getPractitionerName() : "";

    OrderDAO1 dao;
    uint64_t total = 0;
    auto resultList = dao.queryOrderList(
        query->visitId.getValue(""),
        query->patientId.getValue(""),
        query->pageIndex,
        query->pageSize,
        total,
        doctorName
    );

    pageDto->total = total;
    pageDto->calcPages();
    for (auto& item : resultList) {
        pageDto->addData(item);
    }

    return pageDto;
}

// ========== 新增医嘱 ==========
std::string OrderService1::addOrder(const OrderAddDTO1::Wrapper& dto,
    const PayloadDTO* payload,
    std::string& errorMsg)
{
    auto ctx = buildContext(payload);
    if (!ctx.valid) {
        errorMsg = "User context is invalid, please login again";
        return "";
    }

    // ===== 1. Validate ORDER_TYPE =====
    std::string orderType = dto->orderType.getValue("");
    if (!validateEnumCode("ORDER_TYPE", orderType, errorMsg)) {
        return "";
    }

    // ===== 2. Validate EXPENSE_TYPE (validation only, not stored) =====
    std::string expenseType = dto->expenseType.getValue("");
    if (!expenseType.empty()) {
        std::string expenseName;
        if (!validateEnumCode("EXPENSE_TYPE", expenseType, expenseName, errorMsg)) {
            return "";
        }
    }

    // ===== 3. Validate SKIN_TEST =====
    bool isSkinTest = dto->isSkinTest.getValue(false);
    std::string skinTestCode = isSkinTest ? "1" : "0";
    if (!validateEnumCode("SKIN_TEST", skinTestCode, errorMsg)) {
        return "";
    }

    // ===== 4. Validate ORDER_STATUS (DRAFT) =====
    std::string draftStatus = "DRAFT";
    if (!validateEnumCode("ORDER_STATUS", draftStatus, errorMsg)) {
        return "";
    }

    UuidFacade uuid;
    std::string now = SimpleDateTimeFormat::format();
    std::string orderId = uuid.genUuid();

    OrderDAO1 dao;

    if (orderType == "medication") {
        std::string medicationId = dao.getMedicationIdByBusNo(dto->itemCode.getValue(""));
        if (medicationId.empty()) {
            errorMsg = "Medication not found: " + dto->itemCode.getValue("");
            return "";
        }

        MedicationRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(ctx.tenantId);
        data.setRequesterId(ctx.practitionerId);
        data.setOrgId(ctx.orgId);
        data.setCreatedBy(ctx.username);
        data.setUpdatedBy(ctx.username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum(draftStatus);
        data.setEncounterId(dto->visitId.getValue(""));
        data.setPatientId(dto->patientId.getValue(""));
        data.setMedicationId(medicationId);
        data.setBusNo(dto->itemCode.getValue(""));
        data.setQuantity(dto->quantity.getValue(0));
        data.setMethodCode(dto->routeCode.getValue(""));
        data.setRateCode(dto->frequency.getValue(""));
        data.setDose(std::stod(dto->dosage.getValue("0")));
        data.setSkinTestFlag(isSkinTest ? 1 : 0);
        data.setRemark(dto->remark.getValue(""));
        data.setEffectiveDoseStart(now);
        data.setTherapyEnum("TESTORDER");
        data.setUnitCode("8");

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        data.setEffectiveDoseEnd(useDays > 0 ? addDaysToDate(now, useDays) : now);

        if (dao.insertMedicationOrder(data)) {
            auditLog(ctx, orderId, "medication", "ADD", "", dto->itemCode.getValue(""), {}, "Add medication order");
            errorMsg = "";
            return orderId;
        }
        else {
            errorMsg = "Failed to save medication order";
            return "";
        }
    }
    else if (orderType == "service") {
        std::string activityId = dao.getActivityIdByBusNo(dto->itemCode.getValue(""));
        if (activityId.empty()) {
            errorMsg = "Service not found: " + dto->itemCode.getValue("");
            return "";
        }

        ServiceRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(ctx.tenantId);
        data.setRequesterId(ctx.practitionerId);
        data.setOrgId(ctx.orgId);
        data.setCreatedBy(ctx.username);
        data.setUpdatedBy(ctx.username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum(draftStatus);
        data.setEncounterId(dto->visitId.getValue(""));
        data.setPatientId(dto->patientId.getValue(""));
        data.setActivityId(activityId);
        data.setBusNo(dto->itemCode.getValue(""));
        data.setQuantity(dto->quantity.getValue(0));
        data.setRateCode(dto->frequency.getValue(""));
        data.setReasonText(dto->diagnosis.getValue(""));
        data.setRemark(dto->remark.getValue(""));
        data.setOccurrenceStartTime(now);

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        data.setOccurrenceEndTime(useDays > 0 ? addDaysToDate(now, useDays) : now);

        if (dao.insertServiceOrder(data)) {
            auditLog(ctx, orderId, "service", "ADD", "", dto->itemCode.getValue(""), {}, "Add service order");
            errorMsg = "";
            return orderId;
        }
        else {
            errorMsg = "Failed to save service order";
            return "";
        }
    }
    else if (orderType == "device") {
        std::string deviceDefId = dao.getDeviceDefIdByBusNo(dto->itemCode.getValue(""));
        if (deviceDefId.empty()) {
            errorMsg = "Device not found: " + dto->itemCode.getValue("");
            return "";
        }

        DeviceRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(ctx.tenantId);
        data.setRequesterId(ctx.practitionerId);
        data.setOrgId(ctx.orgId);
        data.setCreatedBy(ctx.username);
        data.setUpdatedBy(ctx.username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum(draftStatus);
        data.setEncounterId(dto->visitId.getValue(""));
        data.setPatientId(dto->patientId.getValue(""));
        data.setDeviceDefId(deviceDefId);
        data.setBusNo(dto->itemCode.getValue(""));
        data.setQuantity(dto->quantity.getValue(0));
        data.setRateCode(dto->frequency.getValue(""));
        data.setRemark(dto->remark.getValue(""));
        data.setUseStartTime(now);

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        data.setUseEndTime(useDays > 0 ? addDaysToDate(now, useDays) : now);

        if (dao.insertDeviceOrder(data)) {
            auditLog(ctx, orderId, "device", "ADD", "", dto->itemCode.getValue(""), {}, "Add device order");
            errorMsg = "";
            return orderId;
        }
        else {
            errorMsg = "Failed to save device order";
            return "";
        }
    }

    errorMsg = "Unsupported order type: " + orderType;
    return "";
}

// ========== 修改医嘱 ==========
bool OrderService1::modifyOrder(const OrderModifyDTO1::Wrapper& dto,
    const PayloadDTO* payload,
    std::string& errorMsg)
{
    auto ctx = buildContext(payload);
    if (!ctx.valid) {
        errorMsg = "User context is invalid, please login again";
        return false;
    }

    // ===== Validate SKIN_TEST (if provided) =====
    if (dto->isSkinTest) {
        std::string skinTestCode = dto->isSkinTest.getValue(false) ? "1" : "0";
        if (!validateEnumCode("SKIN_TEST", skinTestCode, errorMsg)) {
            return false;
        }
    }

    OrderDAO1 dao;
    std::string orderId = dto->orderId.getValue("");

    // 先查药品表
    auto medOrder = dao.selectMedicationOrderById(orderId);
    if (medOrder) {
        if (medOrder->getStatusEnum() != "DRAFT") {
            errorMsg = "Order status is not DRAFT, modification not allowed";
            return false;
        }
        if (medOrder->getRequesterId() != ctx.practitionerId) {
            errorMsg = "You can only modify orders created by yourself";
            return false;
        }

        std::string beforeData = "{\"dosage\":\"" + std::to_string(medOrder->getDose()) +
            "\",\"frequency\":\"" + medOrder->getRateCode() +
            "\",\"routeCode\":\"" + medOrder->getMethodCode() +
            "\",\"useDays\":\"" + medOrder->getEffectiveDoseEnd() +
            "\",\"quantity\":\"" + std::to_string(medOrder->getQuantity()) +
            "\",\"isSkinTest\":\"" + std::to_string(medOrder->getSkinTestFlag()) +
            "\",\"remark\":\"" + medOrder->getRemark() + "\"}";

        std::list<std::string> changedFields;
        if (dto->dosage) changedFields.push_back("dosage");
        if (dto->frequency) changedFields.push_back("frequency");
        if (dto->routeCode) changedFields.push_back("routeCode");
        if (dto->useDays) changedFields.push_back("useDays");
        if (dto->quantity) changedFields.push_back("quantity");
        if (dto->isSkinTest) changedFields.push_back("isSkinTest");
        if (dto->remark) changedFields.push_back("remark");

        medOrder->setMethodCode(dto->routeCode.getValue(""));
        medOrder->setRateCode(dto->frequency.getValue(""));
        medOrder->setDose(std::stod(dto->dosage.getValue("0")));
        medOrder->setSkinTestFlag(dto->isSkinTest.getValue(false) ? 1 : 0);
        medOrder->setQuantity(dto->quantity.getValue(0));
        medOrder->setRemark(dto->remark.getValue(""));
        medOrder->setUpdatedBy(ctx.username);
        medOrder->setUpdatedAt(SimpleDateTimeFormat::format());
        medOrder->setStatusChangedTime(SimpleDateTimeFormat::format());
        medOrder->setPerformLocation("123");
        medOrder->setPerformOrg("12345");
        medOrder->setReqAuthoredTime(SimpleDateTimeFormat::format());
        medOrder->setCheckTime(SimpleDateTimeFormat::format());

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = medOrder->getEffectiveDoseStart();
            medOrder->setEffectiveDoseEnd(addDaysToDate(startTime, useDays));
        }

        if (dao.updateMedicationOrder(*medOrder)) {
            std::string afterData = "{\"dosage\":\"" + std::to_string(medOrder->getDose()) +
                "\",\"frequency\":\"" + medOrder->getRateCode() +
                "\",\"routeCode\":\"" + medOrder->getMethodCode() +
                "\",\"useDays\":\"" + medOrder->getEffectiveDoseEnd() +
                "\",\"quantity\":\"" + std::to_string(medOrder->getQuantity()) +
                "\",\"isSkinTest\":\"" + std::to_string(medOrder->getSkinTestFlag()) +
                "\",\"remark\":\"" + medOrder->getRemark() + "\"}";
            auditLog(ctx, orderId, "medication", "MODIFY", beforeData, afterData, changedFields, "Modify medication order");
            errorMsg = "";
            return true;
        }
        else {
            errorMsg = "Failed to update medication order";
            return false;
        }
    }

    // 查诊疗表
    auto svcOrder = dao.selectServiceOrderById(orderId);
    if (svcOrder) {
        if (svcOrder->getStatusEnum() != "DRAFT") {
            errorMsg = "Order status is not DRAFT, modification not allowed";
            return false;
        }
        if (svcOrder->getRequesterId() != ctx.practitionerId) {
            errorMsg = "You can only modify orders created by yourself";
            return false;
        }

        std::string beforeData = "{\"frequency\":\"" + svcOrder->getRateCode() +
            "\",\"quantity\":\"" + std::to_string(svcOrder->getQuantity()) +
            "\",\"remark\":\"" + svcOrder->getRemark() + "\"}";

        std::list<std::string> changedFields;
        if (dto->frequency) changedFields.push_back("frequency");
        if (dto->quantity) changedFields.push_back("quantity");
        if (dto->remark) changedFields.push_back("remark");

        svcOrder->setRateCode(dto->frequency.getValue(""));
        svcOrder->setQuantity(dto->quantity.getValue(0));
        svcOrder->setRemark(dto->remark.getValue(""));
        svcOrder->setUpdatedBy(ctx.username);
        svcOrder->setUpdatedAt(SimpleDateTimeFormat::format());

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = svcOrder->getOccurrenceStartTime();
            svcOrder->setOccurrenceEndTime(addDaysToDate(startTime, useDays));
        }

        if (dao.updateServiceOrder(*svcOrder)) {
            std::string afterData = "{\"frequency\":\"" + svcOrder->getRateCode() +
                "\",\"quantity\":\"" + std::to_string(svcOrder->getQuantity()) +
                "\",\"remark\":\"" + svcOrder->getRemark() + "\"}";
            auditLog(ctx, orderId, "service", "MODIFY", beforeData, afterData, changedFields, "Modify service order");
            errorMsg = "";
            return true;
        }
        else {
            errorMsg = "Failed to update service order";
            return false;
        }
    }

    // 查耗材表
    auto devOrder = dao.selectDeviceOrderById(orderId);
    if (devOrder) {
        if (devOrder->getStatusEnum() != "DRAFT") {
            errorMsg = "Order status is not DRAFT, modification not allowed";
            return false;
        }
        if (devOrder->getRequesterId() != ctx.practitionerId) {
            errorMsg = "You can only modify orders created by yourself";
            return false;
        }

        std::string beforeData = "{\"frequency\":\"" + devOrder->getRateCode() +
            "\",\"quantity\":\"" + std::to_string(devOrder->getQuantity()) +
            "\",\"remark\":\"" + devOrder->getRemark() + "\"}";

        std::list<std::string> changedFields;
        if (dto->frequency) changedFields.push_back("frequency");
        if (dto->quantity) changedFields.push_back("quantity");
        if (dto->remark) changedFields.push_back("remark");

        devOrder->setRateCode(dto->frequency.getValue(""));
        devOrder->setQuantity(dto->quantity.getValue(0));
        devOrder->setRemark(dto->remark.getValue(""));
        devOrder->setUpdatedBy(ctx.username);
        devOrder->setUpdatedAt(SimpleDateTimeFormat::format());

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = devOrder->getUseStartTime();
            devOrder->setUseEndTime(addDaysToDate(startTime, useDays));
        }

        if (dao.updateDeviceOrder(*devOrder)) {
            std::string afterData = "{\"frequency\":\"" + devOrder->getRateCode() +
                "\",\"quantity\":\"" + std::to_string(devOrder->getQuantity()) +
                "\",\"remark\":\"" + devOrder->getRemark() + "\"}";
            auditLog(ctx, orderId, "device", "MODIFY", beforeData, afterData, changedFields, "Modify device order");
            errorMsg = "";
            return true;
        }
        else {
            errorMsg = "Failed to update device order";
            return false;
        }
    }

    errorMsg = "Order not found: " + orderId;
    return false;
}