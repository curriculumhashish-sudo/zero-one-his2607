// dao/order/OrderDAO1.h
#pragma once
#ifndef _ORDER_DAO_1_H_
#define _ORDER_DAO_1_H_

#include "BaseDAO.h"
#include "../../domain/do/order/MedicationRequestDO1.h"
#include "../../domain/do/order/ServiceRequestDO1.h"
#include "../../domain/do/order/DeviceRequestDO1.h"
#include "../../domain/do/order/MedicationDefinitionDO1.h"
#include "../../domain/do/order/ActivityDefinitionDO1.h"
#include "../../domain/do/order/DeviceDefinitionDO1.h"
#include "../../domain/do/order/ChargeItemDefinitionDO1.h"
#include "../../domain/do/order/PractitionerDO1.h"
#include "../../domain/dto/order/OrderDetailDTO1.h"

/**
 * 医嘱相关数据库操作
 * 重构：移除 getPractitionerName，医生姓名由 Service 层从 Payload 传入
 */
class OrderDAO1 : public BaseDAO
{
private:
    /**
     * 获取物品单价（从 adm_charge_item_definition 查询）
     */
    double getUnitPrice(const std::string& instanceTable, const std::string& instanceId);

public:
    /**
     * 通过 bus_no 查询药品定义 ID
     */
    std::string getMedicationIdByBusNo(const std::string& busNo);

    /**
     * 通过 bus_no 查询诊疗定义 ID
     */
    std::string getActivityIdByBusNo(const std::string& busNo);

    /**
     * 通过 bus_no 查询耗材定义 ID
     */
    std::string getDeviceDefIdByBusNo(const std::string& busNo);

    /**
     * 查询患者医嘱列表（合并药品、诊疗、耗材医嘱）
     * @param doctorName 医生姓名（由 Service 层从 Payload 传入，不再查数据库）
     */
    std::list<OrderDetailDTO1::Wrapper> queryOrderList(
        const std::string& encounterId,
        const std::string& patientId,
        uint64_t pageIndex,
        uint64_t pageSize,
        uint64_t& total,
        const std::string& doctorName = ""  // 新增参数
    );

    /**
     * 插入药品医嘱
     */
    bool insertMedicationOrder(const MedicationRequestDO1& data);

    /**
     * 插入诊疗医嘱
     */
    bool insertServiceOrder(const ServiceRequestDO1& data);

    /**
     * 插入耗材医嘱
     */
    bool insertDeviceOrder(const DeviceRequestDO1& data);

    /**
     * 更新药品医嘱
     */
    bool updateMedicationOrder(const MedicationRequestDO1& data);

    /**
     * 更新诊疗医嘱
     */
    bool updateServiceOrder(const ServiceRequestDO1& data);

    /**
     * 更新耗材医嘱
     */
    bool updateDeviceOrder(const DeviceRequestDO1& data);

    /**
     * 根据ID查询药品医嘱
     */
    PtrMedicationRequestDO1 selectMedicationOrderById(const std::string& id);

    /**
     * 根据ID查询诊疗医嘱
     */
    PtrServiceRequestDO1 selectServiceOrderById(const std::string& id);

    /**
     * 根据ID查询耗材医嘱
     */
    PtrDeviceRequestDO1 selectDeviceOrderById(const std::string& id);
};

#endif // !_ORDER_DAO_1_H_