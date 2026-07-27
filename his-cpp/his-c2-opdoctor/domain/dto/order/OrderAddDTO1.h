#pragma once
#ifndef _ORDER_ADD_DTO_1_H_
#define _ORDER_ADD_DTO_1_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 新增医嘱信息
 * 演示项目中是通过搜索框选中药品或检查，然后医生手动录入加工字段后提交信息
 * 以此来实现新增医嘱的功能
 *
 * 根据 orderType 字段值决定写入不同表：
 * - medication -> med_medication_request
 * - service -> wkf_service_request
 * - device -> wkf_device_request
 */
class OrderAddDTO1 : public oatpp::DTO {
    DTO_INIT(OrderAddDTO1, DTO);

    // === 上下文（必传） ===
    API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("createOrder.order.field.visitid"), true);          // 本次就诊ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("createOrder.order.field.patientid"), true);    // 患者ID
    API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("createOrder.order.field.diagnosis"), true);    // 当前诊断（如“感冒”）
    API_DTO_FIELD_REQUIRE(String, expenseType, ZH_WORDS_GETTER("createOrder.order.field.expenseType"), true);// 费用性质（自费/医保/公费）

    // === 来自搜索框的药品/项目信息（前端回传） ===
    API_DTO_FIELD_REQUIRE(String, itemCode, ZH_WORDS_GETTER("createOrder.order.field.itemCode"), true);      // 项目编码（唯一主键）
    API_DTO_FIELD_REQUIRE(String, itemName, ZH_WORDS_GETTER("createOrder.order.field.itemName"), true);      // 项目名称
    API_DTO_FIELD_REQUIRE(String, spec, ZH_WORDS_GETTER("createOrder.order.field.spec"), true);              // 规格（2ml:15mg）
    API_DTO_FIELD_REQUIRE(Float64, unitPrice, ZH_WORDS_GETTER("createOrder.order.field.unitPrice"), true);   // 单价（从目录带出）
    API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("createOrder.order.field.orderType"), true);    // 类型（medication/service/device）

    // === 医生手动录入的临床决策 ===
    API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("createOrder.order.field.dosage"), false);         // 每次用量（如 "2ml"，诊疗可为空）
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("createOrder.order.field.frequency"), false);   // 频次（诊疗可为空）
    API_DTO_FIELD_REQUIRE(String, routeCode, ZH_WORDS_GETTER("createOrder.order.field.routeCode"), false);   // 给药途径（诊疗可为空）
    API_DTO_FIELD_REQUIRE(Int32, useDays, ZH_WORDS_GETTER("createOrder.order.field.useDays"), false);        // 使用天数（如 7）
    API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("createOrder.order.field.quantity"), true);       // 总数量
    API_DTO_FIELD_REQUIRE(Boolean, isSkinTest, ZH_WORDS_GETTER("createOrder.order.field.isSkinTest"), false);// 是否皮试（true/false，诊疗可为空）
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("createOrder.order.field.remark"), false);         // 备注

    // === 关联 PayloadDTO 负载数据对象 ===
    //CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
protected: const PayloadDTO* _payload;
public: const PayloadDTO* getPayload(void) const { return _payload; }
public: void setPayload(const PayloadDTO* var) { _payload = var; }
};

#include OATPP_CODEGEN_END(DTO)
#endif