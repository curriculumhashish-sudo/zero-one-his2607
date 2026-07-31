// service/order/OrderService1.h
#pragma once
#ifndef _ORDER_SERVICE_1_H_
#define _ORDER_SERVICE_1_H_

#include "domain/vo/order/OrderVO1.h"
#include "domain/query/order/OrderQuery1.h"
#include "domain/dto/order/OrderAddDTO1.h"
#include "domain/dto/order/OrderModifyDTO1.h"
#include "domain/dto/order/OrderDetailDTO1.h"
#include "domain/dto/PayloadDTO.h"
#include "dao/order/EnumDAO.h"
#include <memory>
#include <vector>

// 前向声明
class RedisClient;
class MongoClient;
class SessionService;
class OrderAuditDAO1;
class EnumDAO;

/**
 * 医嘱服务实现
 */
class OrderService1
{
private:
    std::shared_ptr<RedisClient> m_redisClient;
    std::shared_ptr<MongoClient> m_mongoClient;
    std::shared_ptr<SessionService> m_sessionService;
    std::shared_ptr<OrderAuditDAO1> m_auditDAO;
    std::shared_ptr<EnumDAO> m_enumDAO;

    struct ServiceContext {
        std::string userId;
        std::string username;
        std::string practitionerId;
        std::string practitionerName;
        std::string tenantId;
        std::string orgId;
        bool valid;
    };

    ServiceContext buildContext(const PayloadDTO* payload);

    void auditLog(
        const ServiceContext& ctx,
        const std::string& orderId,
        const std::string& orderType,
        const std::string& operationType,
        const std::string& beforeData = "",
        const std::string& afterData = "",
        const std::list<std::string>& changedFields = {},
        const std::string& remark = ""
    );

    /**
     * 校验枚举值是否存在于 MongoDB
     * @param enumType 枚举类型（如 ORDER_TYPE、EXPENSE_TYPE）
     * @param code 枚举编码
     * @param outName 输出参数，返回枚举名称
     * @param errorMsg 输出参数，返回错误信息
     * @return true 表示存在，false 表示不存在
     */
    bool validateEnumCode(const std::string& enumType, const std::string& code,
        std::string& outName, std::string& errorMsg);

    /**
     * 校验枚举值是否存在，不返回名称
     */
    bool validateEnumCode(const std::string& enumType, const std::string& code,
        std::string& errorMsg);

public:
    OrderService1(std::shared_ptr<RedisClient> redisClient,
        std::shared_ptr<MongoClient> mongoClient);
    ~OrderService1() = default;

    // 查询医嘱列表
    OrderDetailPageDTO1::Wrapper queryOrderList(const OrderQuery1::Wrapper& query,
        const PayloadDTO* payload);

    // 新增医嘱 - 成功返回订单ID，失败返回空字符串，errorMsg 包含错误信息
    std::string addOrder(const OrderAddDTO1::Wrapper& dto,
        const PayloadDTO* payload,
        std::string& errorMsg);

    // 修改医嘱 - 成功返回 true，失败返回 false，errorMsg 包含错误信息
    bool modifyOrder(const OrderModifyDTO1::Wrapper& dto,
        const PayloadDTO* payload,
        std::string& errorMsg);

    // 获取枚举值列表（供前端下拉使用）
    std::vector<EnumValue> getEnumValues(const std::string& enumType);
};

#endif // !_ORDER_SERVICE_1_H_