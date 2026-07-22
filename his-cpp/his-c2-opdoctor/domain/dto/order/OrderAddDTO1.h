#pragma once
#ifndef _ORDER_ADD_DTO_H_
#define _ORDER_ADD_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section

/**
 * 新增医嘱信息
 * 演示项目中是通过搜索框选中药品或检查，然后医生手动录入加工字段（如用量、频次、途径等）后提交信息
 * 以此来实现新增医嘱的功能
 */
class OrderAddDTO1 : public oatpp::DTO {
	DTO_INIT(OrderAddDTO1, DTO);

	// === 上下文（必传） ===
	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("prescription1.order.field.visitid"), true);    // 本次就诊ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("prescription1.order.field.patientid"), true);      // 患者ID
	API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("prescription1.order.field.diagnosis"), true);      // 当前诊断（如“感冒”）
	API_DTO_FIELD_REQUIRE(String, expenseType, ZH_WORDS_GETTER("prescription1.order.field.expenseType"), true);      // 费用性质（自费/医保/公费）


	// === 来自搜索框的药品/项目信息（前端回传） ===
	API_DTO_FIELD_REQUIRE(String, itemCode, ZH_WORDS_GETTER("prescription1.order.field.itemCode"), true);       // 项目编码（唯一主键）
	API_DTO_FIELD_REQUIRE(String, itemName, ZH_WORDS_GETTER("prescription1.order.field.itemName"), true);       // 项目名称
	API_DTO_FIELD_REQUIRE(String, spec, ZH_WORDS_GETTER("prescription1.order.field.spec"), true);           // 规格（2ml:15mg）
	API_DTO_FIELD_REQUIRE(Float64, unitPrice, ZH_WORDS_GETTER("prescription1.order.field.unitPrice"), true);     // 单价（从目录带出）
	API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("prescription1.order.field.orderType"), true);      // 类型（西药/检验/检查）

	// === 医生手动录入的临床决策 ===
	API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("prescription1.order.field.dosage"), true);         // 每次用量（如 "2ml"）
	API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("prescription1.order.field.frequency"), true);  // 频次
	API_DTO_FIELD_REQUIRE(String, routeCode, ZH_WORDS_GETTER("prescription1.order.field.routeCode"), true);      // 给药途径
	API_DTO_FIELD_REQUIRE(Int32, useDays, ZH_WORDS_GETTER("prescription1.order.field.useDays"), true);         // 使用天数（如 7）
	API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("prescription1.order.field.quantity"), true);        // 总数量（前端计算好）
	API_DTO_FIELD_REQUIRE(Boolean, isSkinTest, ZH_WORDS_GETTER("prescription1.order.field.isSkinTest"), true);    // 是否皮试（true/false）
	API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("prescription1.order.field.remark"), false);         // 备注
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif 