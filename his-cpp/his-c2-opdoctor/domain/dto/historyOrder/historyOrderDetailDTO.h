#pragma once
#ifndef _HISTORY_ORDER_DETAIL_DTO_H_
#define _HISTORY_ORDER_DETAIL_DTO_H_

#include "../../GlobalInclude.h"			// 项目全局公共头文件

#include OATPP_CODEGEN_BEGIN(DTO)			// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱明细单条DTO
 * 对应右侧医嘱表格展示字段
 */
class HistoryOrderDetailDTO : public oatpp::DTO {
	DTO_INIT(HistoryOrderDetailDTO, DTO);		// DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(String, orderId, ZH_WORDS_GETTER("historyOrder.field.orderId"), true);	// 医嘱唯一ID（业务主键，界面不展示）
	API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("historyOrder.field.orderType"), true);	// 医嘱类型，如检验、检查，对应表格“类型”列
	API_DTO_FIELD_REQUIRE(String, orderName, ZH_WORDS_GETTER("historyOrder.field.orderName"), true);	// 医嘱名称，如肝功13项，对应表格“医嘱内容”列主标题
	API_DTO_FIELD_REQUIRE(Int32, quantity, ZH_WORDS_GETTER("historyOrder.field.quantity"), true);	// 开立总次数，对应“数量X次”
	API_DTO_FIELD_REQUIRE(Int32, executedCount, ZH_WORDS_GETTER("historyOrder.field.executedCount"), true); // 已执行次数，对应“执行X次”
	API_DTO_FIELD(String, sampleType, ZH_WORDS_GETTER("historyOrder.field.sampleType"), false);		// 样本类型，如血，检验类医嘱展示
	API_DTO_FIELD_REQUIRE(String, executeHospital, ZH_WORDS_GETTER("historyOrder.field.executeHospital"), true); // 执行医院，医嘱内容列底部展示
	API_DTO_FIELD_REQUIRE(String, createTime, ZH_WORDS_GETTER("historyOrder.field.createTime"), true);	// 开立时间，对应表格“开立时间”列
	API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("historyOrder.field.diagnosis"), true);		// 对应诊断，对应表格“诊断”列
	API_DTO_FIELD_REQUIRE(String, doctorName, ZH_WORDS_GETTER("historyOrder.field.doctorName"), true);	// 开立医生，对应表格“医生”列
	API_DTO_FIELD(Boolean, isSkinTest, ZH_WORDS_GETTER("historyOrder.field.isSkinTest"), false);			// 是否皮试，对应表格“皮试”列，无则显示—
};

/**
 * 历史医嘱明细分页DTO
 * 封装分页参数与医嘱明细列表，供VO层包装返回
 */
class HistoryOrderDetailPageDTO : public oatpp::DTO {
	DTO_INIT(HistoryOrderDetailPageDTO, DTO);	// DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(Int64, total, ZH_WORDS_GETTER("common.total"), true);		// 总记录条数
	API_DTO_FIELD_REQUIRE(Int32, pageNum, ZH_WORDS_GETTER("common.pageNum"), true);	// 当前页码
	API_DTO_FIELD_REQUIRE(Int32, pageSize, ZH_WORDS_GETTER("common.pageSize"), true);	// 每页显示条数
	API_DTO_FIELD_REQUIRE(List<HistoryOrderDetailDTO::Wrapper>, list, ZH_WORDS_GETTER("common.list"), true); // 医嘱明细数据列表
};

#include OATPP_CODEGEN_END(DTO)			// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_DETAIL_DTO_H_	// 头文件保护宏结束
