#pragma once
#ifndef _ORDER_MODIFY_DTO_1_H_
#define _ORDER_MODIFY_DTO_1_H_

#include "domain/dto/order/OrderAddDTO1.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 修改医嘱信息
 * 前置校验：
 * 1. 该医嘱状态必须为 DRAFT（待签发）
 * 2. 当前登录医生 ID 必须等于该医嘱的 requester_id（只能改自己的）
 *
 * 允许修改的字段仅限临床决策部分：
 * - dosage、frequency、routeCode、useDays、quantity、isSkinTest、remark
 *
 * 禁止修改的字段（忽略前端传入）：
 * - orderId（只能用于定位，不能改值）
 * - visitId、patientId、itemCode、itemName、spec、unitPrice、orderType
 * - status（状态由后端控制，不允许前端修改）
 * - totalPrice（由后端重算，不信任前端传值）
 */
class OrderModifyDTO1 : public oatpp::DTO {
    DTO_INIT(OrderModifyDTO1, DTO);

    // === 定位字段（必传，用于定位要修改的医嘱） ===
    API_DTO_FIELD_REQUIRE(String, orderId, ZH_WORDS_GETTER("createOrder.order.field.orderid"), true);    // 医嘱ID
    API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("createOrder.order.field.visitid"), true);    // 就诊ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("createOrder.order.field.patientid"), true);// 患者ID

    // === 允许修改的字段 ===
    API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("createOrder.order.field.dosage"), false);         // 每次用量
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("createOrder.order.field.frequency"), false);   // 频次
    API_DTO_FIELD_REQUIRE(String, routeCode, ZH_WORDS_GETTER("createOrder.order.field.routeCode"), false);   // 给药途径
    API_DTO_FIELD_REQUIRE(Int32, useDays, ZH_WORDS_GETTER("createOrder.order.field.useDays"), false);        // 使用天数
    API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("createOrder.order.field.quantity"), true);       // 总数量
    API_DTO_FIELD_REQUIRE(Boolean, isSkinTest, ZH_WORDS_GETTER("createOrder.order.field.isSkinTest"), false);// 是否皮试
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("createOrder.order.field.remark"), false);         // 备注

    // === 关联 PayloadDTO 负载数据对象 ===
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)
#endif