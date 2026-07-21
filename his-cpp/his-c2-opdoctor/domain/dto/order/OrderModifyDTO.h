#pragma once
#ifndef _ORDER_MODIFY_DTO_H_
#define _ORDER_MODIFY_DTO_H_

#include "domain/dto/order/OrderAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section

class OrderModifyDTO : public OrderAddDTO {
	DTO_INIT(OrderModifyDTO, OrderAddDTO);
	// === 上下文（必传） ===
	API_DTO_FIELD_REQUIRE(String, orderId, ZH_WORDS_GETTER("order.field.orderid"), true);    // 医嘱ID
	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("order.field.visitid"), true);    // 就诊ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("order.field.patientid"), true);    // 患者ID
	// === 医生手动录入的临床决策 ===
	API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("order.field.dosage"), true);         // 每次用量（如 "2ml"）
	API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("order.field.frequency"), true);  // 频次
	API_DTO_FIELD_REQUIRE(String, routeCode, ZH_WORDS_GETTER("order.field.routeCode"), true);      // 给药途径
	API_DTO_FIELD_REQUIRE(Int32, useDays, ZH_WORDS_GETTER("order.field.useDays"), true);         // 使用天数（如 7）
	API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("order.field.quantity"), true);        // 总数量
	API_DTO_FIELD_REQUIRE(Boolean, isSkinTest, ZH_WORDS_GETTER("order.field.isSkinTest"), true);    // 是否皮试（true/false）
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("order.field.status"), true);         // 状态（待签发/已完成）
	API_DTO_FIELD_REQUIRE(Float64, totalPrice, ZH_WORDS_GETTER("order.field.totalPrice"), true);     // 总价
	API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("order.field.remark"), false);         // 备注
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif