#pragma once
#ifndef _HISTORY_ORDER_LIST_DTO_H_
#define _HISTORY_ORDER_LIST_DTO_H_

#include "../../GlobalInclude.h"			// 项目全局公共头文件

#include OATPP_CODEGEN_BEGIN(DTO)			// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱就诊记录单条DTO
 * 对应左侧就诊记录卡片展示字段
 */
class HistoryOrderListDTO : public oatpp::DTO {
	DTO_INIT(HistoryOrderListDTO, DTO);		// DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("historyOrder.field.visitId"), true);	// 就诊记录唯一ID（业务主键，界面不展示）
	API_DTO_FIELD_REQUIRE(String, visitTime, ZH_WORDS_GETTER("historyOrder.field.visitTime"), true);	// 就诊时间，界面卡片顶部展示
	API_DTO_FIELD_REQUIRE(String, visitType, ZH_WORDS_GETTER("historyOrder.field.visitType"), true);	// 就诊类型，如门诊
	API_DTO_FIELD_REQUIRE(String, mainDiagnosis, ZH_WORDS_GETTER("historyOrder.field.mainDiagnosis"), true); // 主诊断，界面卡片展示
	API_DTO_FIELD_REQUIRE(String, doctorName, ZH_WORDS_GETTER("historyOrder.field.doctorName"), true);	// 接诊医生，界面卡片底部展示
	API_DTO_FIELD_REQUIRE(Int32, drugCount, ZH_WORDS_GETTER("historyOrder.field.drugCount"), true);		// 药品类医嘱数量，对应卡片统计“药”
	API_DTO_FIELD_REQUIRE(Int32, examCount, ZH_WORDS_GETTER("historyOrder.field.examCount"), true);		// 检验检查类医嘱数量，对应卡片统计“诊”
	API_DTO_FIELD_REQUIRE(Int32, materialCount, ZH_WORDS_GETTER("historyOrder.field.materialCount"), true); // 耗材类医嘱数量，对应卡片统计“材”
};

/**
 * 历史医嘱就诊记录分页DTO
 * 封装分页参数与就诊记录列表，供VO层包装返回
 */
class HistoryOrderListPageDTO : public oatpp::DTO {
	DTO_INIT(HistoryOrderListPageDTO, DTO);		// DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(Int64, total, ZH_WORDS_GETTER("common.total"), true);		// 总记录条数
	API_DTO_FIELD_REQUIRE(Int32, pageNum, ZH_WORDS_GETTER("common.pageNum"), true);	// 当前页码
	API_DTO_FIELD_REQUIRE(Int32, pageSize, ZH_WORDS_GETTER("common.pageSize"), true);	// 每页显示条数
	API_DTO_FIELD_REQUIRE(List<HistoryOrderListDTO::Wrapper>, list, ZH_WORDS_GETTER("common.list"), true); // 就诊记录数据列表
};

#include OATPP_CODEGEN_END(DTO)			// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_LIST_DTO_H_		// 头文件保护宏结束
