#pragma once
#ifndef _ORDER_Detail_DTO_1_H_
#define _ORDER_Detail_DTO_1_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section 

/**
* 返回医嘱详情信息，用于前端展示
*/
class OrderDetailDTO1 : public oatpp::DTO {
	DTO_INIT(OrderDetailDTO1, DTO);
	API_DTO_FIELD_REQUIRE(String, orderId, ZH_WORDS_GETTER("createOrder.order.field.orderid"), true);    // 医嘱ID
	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("createOrder.order.field.visitid"), true);    // 就诊ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("createOrder.order.field.patientid"), true);    // 患者ID
	API_DTO_FIELD_REQUIRE(String, doctorId, ZH_WORDS_GETTER("createOrder.order.field.doctorId"), true);       // 后端回填，用于权限判断
	API_DTO_FIELD_REQUIRE(String, doctorName, ZH_WORDS_GETTER("createOrder.order.field.doctorName"), true);   // 显示开立医生
	API_DTO_FIELD_REQUIRE(String, itemCode, ZH_WORDS_GETTER("createOrder.order.field.itemCode"), true);       // 项目编码（唯一主键）
	API_DTO_FIELD_REQUIRE(String, itemName, ZH_WORDS_GETTER("createOrder.order.field.itemName"), true);       // 项目名称
	API_DTO_FIELD_REQUIRE(String, spec, ZH_WORDS_GETTER("createOrder.order.field.spec"), true);           // 规格（2ml:15mg）
	API_DTO_FIELD_REQUIRE(Float64, unitPrice, ZH_WORDS_GETTER("createOrder.order.field.unitPrice"), true);     // 单价（从目录带出）
	API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("createOrder.order.field.orderType"), true);      // 类型（西药/检验/检查）
	
	// === 医生手动录入的临床决策 ===
	API_DTO_FIELD_REQUIRE(String, dosage, ZH_WORDS_GETTER("createOrder.order.field.dosage"), true);         // 每次用量（如 "2ml"）
	API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("createOrder.order.field.frequency"), true);  // 频次
	API_DTO_FIELD_REQUIRE(String, routeCode, ZH_WORDS_GETTER("createOrder.order.field.routeCode"), true);      // 给药途径
	API_DTO_FIELD_REQUIRE(Int32, useDays, ZH_WORDS_GETTER("createOrder.order.field.useDays"), true);         // 使用天数（如 7）
	API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("createOrder.order.field.quantity"), true);        // 总数量（前端计算好）
	API_DTO_FIELD_REQUIRE(Boolean, isSkinTest, ZH_WORDS_GETTER("createOrder.order.field.isSkinTest"), true);    // 是否皮试（true/false）
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("createOrder.order.field.status"), true);         // 状态（待签发/已完成）
	API_DTO_FIELD_REQUIRE(String, createTime, ZH_WORDS_GETTER("createOrder.order.field.createTime"), true);         // 开立时间
	API_DTO_FIELD_REQUIRE(Float64, totalPrice, ZH_WORDS_GETTER("createOrder.order.field.totalPrice"), true);     // 总价
	API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("createOrder.order.field.remark"), false);         // 备注

	// ===== 分组支持（用于拆组/并组） =====
	API_DTO_FIELD_REQUIRE(String, groupId, ZH_WORDS_GETTER("createOrder.order.field.groupId"), false);       // 组套ID
	API_DTO_FIELD_REQUIRE(String, parentOrderId, ZH_WORDS_GETTER("createOrder.order.field.parentOrderId"), false); // 父医嘱ID
};
/*
* 分页展示
*/
class OrderDetailPageDTO1 : public PageDTO<OrderDetailDTO1::Wrapper> {
	DTO_INIT(OrderDetailPageDTO1, PageDTO<OrderDetailDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif