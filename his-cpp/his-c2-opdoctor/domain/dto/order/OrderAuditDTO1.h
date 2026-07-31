#pragma once
#ifndef _ORDER_AUDIT_DTO_1_H_
#define _ORDER_AUDIT_DTO_1_H_

#include "../../GlobalInclude.h" 

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 医嘱操作审计日志 DTO
 * 写入 MongoDB order_audit_logs 集合
 */
class OrderAuditDTO1 : public oatpp::DTO {
    DTO_INIT(OrderAuditDTO1, DTO);

    // 医嘱ID
    API_DTO_FIELD_REQUIRE(String, orderId, "医嘱ID", true);

    // 医嘱类型: medication / service / device
    API_DTO_FIELD_REQUIRE(String, orderType, "医嘱类型", true);

    // 操作类型: ADD / MODIFY / EXECUTE / STOP
    API_DTO_FIELD_REQUIRE(String, operationType, "操作类型", true);

    // 操作人ID (practitionerId)
    API_DTO_FIELD_REQUIRE(String, operatorId, "操作人ID", true);

    // 操作人姓名
    API_DTO_FIELD_REQUIRE(String, operatorName, "操作人姓名", true);

    // 操作人机构ID
    API_DTO_FIELD_REQUIRE(String, operatorOrgId, "操作人机构ID", true);

    // 操作时间 (ISO 8601)
    API_DTO_FIELD_REQUIRE(String, operationTime, "操作时间", true);

    // 变更前数据 (JSON 字符串)
    API_DTO_FIELD_REQUIRE(String, beforeData, "变更前数据", false);

    // 变更后数据 (JSON 字符串)
    API_DTO_FIELD_REQUIRE(String, afterData, "变更后数据", false);

    // 变更字段列表
    API_DTO_FIELD_REQUIRE(List<String>, changedFields, "变更字段列表", false);

    // 备注
    API_DTO_FIELD_REQUIRE(String, remark, "备注", false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // _ORDER_AUDIT_DTO_1_H_