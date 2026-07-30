#include "stdafx.h"
#include "OrderService1.h"
#include "../../dao/order/OrderDAO1.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "domain/do/order/MedicationRequestDO1.h"
#include "domain/do/order/ServiceRequestDO1.h"
#include "domain/do/order/DeviceRequestDO1.h"
#include <ctime>
#include <sstream>
#include <iomanip>

/**
 * 简单的日期加减工具函数
 * 将字符串日期解析为 time_t，增加天数，再格式化为字符串
 */
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

OrderDetailPageDTO1::Wrapper OrderService1::queryOrderList(const OrderQuery1::Wrapper& query)
{
    auto pageDto = OrderDetailPageDTO1::createShared();
    pageDto->pageIndex = query->pageIndex;
    pageDto->pageSize = query->pageSize;

    OrderDAO1 dao;
    uint64_t total = 0;
    auto resultList = dao.queryOrderList(
        query->visitId.getValue(""),
        query->patientId.getValue(""),
        query->pageIndex,
        query->pageSize,
        total
    );

    pageDto->total = total;
    pageDto->calcPages();
    for (auto& item : resultList) {
        pageDto->addData(item);
    }

    return pageDto;
}

std::string OrderService1::addOrder(const OrderAddDTO1::Wrapper& dto)
{
    // 从 dto 内部获取 payload
    const PayloadDTO* payload = dto->getPayload();
    if (payload == nullptr) {
        return "";
    }

    UuidFacade uuid;
    std::string now = SimpleDateTimeFormat::format();
    std::string orderId = uuid.genUuid();

    // 从 payload 获取字段
    std::string tenantId = payload->getTenantId();
    std::string requesterId = payload->getId();
    std::string orgId = payload->getOrgId();
    std::string username = payload->getUsername();

    // 根据 orderType 写入不同表
    std::string orderType = dto->orderType.getValue("");
    OrderDAO1 dao;

    if (orderType == "medication") {
        // 通过 bus_no 查询药品定义 ID
        std::string medicationId = dao.getMedicationIdByBusNo(dto->itemCode.getValue(""));
        if (medicationId.empty()) {
            return "";
        }

        MedicationRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(tenantId);
        data.setRequesterId(requesterId);
        data.setOrgId(orgId);
        data.setCreatedBy(username);
        data.setUpdatedBy(username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum("DRAFT");
        data.setEncounterId(dto->visitId.getValue(""));
        data.setPatientId(dto->patientId.getValue(""));
        data.setMedicationId(medicationId);
        data.setBusNo(dto->itemCode.getValue(""));
        data.setQuantity(dto->quantity.getValue(0));
        data.setMethodCode(dto->routeCode.getValue(""));
        data.setRateCode(dto->frequency.getValue(""));
        data.setDose(std::stod(dto->dosage.getValue("0")));
        data.setSkinTestFlag(dto->isSkinTest.getValue(false) ? 1 : 0);
        data.setRemark(dto->remark.getValue(""));
        data.setEffectiveDoseStart(now);
        // 新增字段
		data.setTherapyEnum("TESTORDER");
        data.setUnitCode("8");

        // 计算结束时间：当前时间 + useDays 天
        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            data.setEffectiveDoseEnd(addDaysToDate(now, useDays));
        }
        else {
            data.setEffectiveDoseEnd(now);
        }

        if (dao.insertMedicationOrder(data)) {
            return orderId;
        }
    }
    else if (orderType == "service") {
        // 通过 bus_no 查询诊疗定义 ID
        std::string activityId = dao.getActivityIdByBusNo(dto->itemCode.getValue(""));
        if (activityId.empty()) {
            return "";
        }

        ServiceRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(tenantId);
        data.setRequesterId(requesterId);
        data.setOrgId(orgId);
        data.setCreatedBy(username);
        data.setUpdatedBy(username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum("DRAFT");
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
        if (useDays > 0) {
            data.setOccurrenceEndTime(addDaysToDate(now, useDays));
        }
        else {
            data.setOccurrenceEndTime(now);
        }

        if (dao.insertServiceOrder(data)) {
            return orderId;
        }
    }
    else if (orderType == "device") {
        // 通过 bus_no 查询耗材定义 ID
        std::string deviceDefId = dao.getDeviceDefIdByBusNo(dto->itemCode.getValue(""));
        if (deviceDefId.empty()) {
            return "";
        }

        DeviceRequestDO1 data;
        data.setId(orderId);
        data.setTenantId(tenantId);
        data.setRequesterId(requesterId);
        data.setOrgId(orgId);
        data.setCreatedBy(username);
        data.setUpdatedBy(username);
        data.setCreatedAt(now);
        data.setUpdatedAt(now);
        data.setStatusEnum("DRAFT");
        data.setEncounterId(dto->visitId.getValue(""));
        data.setPatientId(dto->patientId.getValue(""));
        data.setDeviceDefId(deviceDefId);
        data.setBusNo(dto->itemCode.getValue(""));
        data.setQuantity(dto->quantity.getValue(0));
        data.setRateCode(dto->frequency.getValue(""));
        data.setRemark(dto->remark.getValue(""));
        data.setUseStartTime(now);

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            data.setUseEndTime(addDaysToDate(now, useDays));
        }
        else {
            data.setUseEndTime(now);
        }

        if (dao.insertDeviceOrder(data)) {
            return orderId;
        }
    }

    return "";
}

bool OrderService1::modifyOrder(const OrderModifyDTO1::Wrapper& dto)
{
    // 从 dto 内部获取 payload
    const PayloadDTO* payload = dto->getPayload();
    if (payload == nullptr) {
        return false;
    }

    OrderDAO1 dao;
    std::string orderId = dto->orderId.getValue("");

    // 先查药品表
    auto medOrder = dao.selectMedicationOrderById(orderId);
    if (medOrder) {
        if (medOrder->getStatusEnum() != "DRAFT") return false;
        if (medOrder->getRequesterId() != payload->getId()) return false;

        medOrder->setMethodCode(dto->routeCode.getValue(""));
        medOrder->setRateCode(dto->frequency.getValue(""));
        medOrder->setDose(std::stod(dto->dosage.getValue("0")));
        medOrder->setSkinTestFlag(dto->isSkinTest.getValue(false) ? 1 : 0);
        medOrder->setQuantity(dto->quantity.getValue(0));
        medOrder->setRemark(dto->remark.getValue(""));
        medOrder->setUpdatedBy(payload->getUsername());
        medOrder->setUpdatedAt(SimpleDateTimeFormat::format());
        // 新增部分
        medOrder->setStatusChangedTime(SimpleDateTimeFormat::format());
        medOrder->setPerformLocation("123");
        medOrder->setPerformOrg("12345");
        medOrder->setReqAuthoredTime(SimpleDateTimeFormat::format());
        medOrder->setCheckTime(SimpleDateTimeFormat::format());
        // 更新结束时间 cvb6nm687hj9g
        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = medOrder->getEffectiveDoseStart();
            medOrder->setEffectiveDoseEnd(addDaysToDate(startTime, useDays));
        }
        return dao.updateMedicationOrder(*medOrder);
    }

    // 查诊疗表
    auto svcOrder = dao.selectServiceOrderById(orderId);
    if (svcOrder) {
        if (svcOrder->getStatusEnum() != "DRAFT") return false;
        if (svcOrder->getRequesterId() != payload->getId()) return false;

        svcOrder->setRateCode(dto->frequency.getValue(""));
        svcOrder->setQuantity(dto->quantity.getValue(0));
        svcOrder->setRemark(dto->remark.getValue(""));
        svcOrder->setUpdatedBy(payload->getUsername());
        svcOrder->setUpdatedAt(SimpleDateTimeFormat::format());

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = svcOrder->getOccurrenceStartTime();
            svcOrder->setOccurrenceEndTime(addDaysToDate(startTime, useDays));
        }
        return dao.updateServiceOrder(*svcOrder);
    }

    // 查耗材表
    auto devOrder = dao.selectDeviceOrderById(orderId);
    if (devOrder) {
        if (devOrder->getStatusEnum() != "DRAFT") return false;
        if (devOrder->getRequesterId() != payload->getId()) return false;

        devOrder->setRateCode(dto->frequency.getValue(""));
        devOrder->setQuantity(dto->quantity.getValue(0));
        devOrder->setRemark(dto->remark.getValue(""));
        devOrder->setUpdatedBy(payload->getUsername());
        devOrder->setUpdatedAt(SimpleDateTimeFormat::format());

        int useDays = dto->useDays ? dto->useDays.getValue(0) : 0;
        if (useDays > 0) {
            std::string startTime = devOrder->getUseStartTime();
            devOrder->setUseEndTime(addDaysToDate(startTime, useDays));
        }
        return dao.updateDeviceOrder(*devOrder);
    }

    return false;
}