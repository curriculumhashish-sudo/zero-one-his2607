#pragma once
#ifndef _HISTORY_ORDER_LIST_QUERY_H_
#define _HISTORY_ORDER_LIST_QUERY_H_

#include "../../GlobalInclude.h"			// 项目全局公共头文件
#include "domain/query/PageQuery.h"			// 分页查询基类，继承分页参数

#include OATPP_CODEGEN_BEGIN(DTO)			// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱就诊记录列表查询请求
 * 场景：打开历史医嘱弹窗，加载左侧就诊记录列表
 */
class HistoryOrderListQuery : public PageQuery {
	DTO_INIT(HistoryOrderListQuery, PageQuery);	// DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("historyOrder.field.patientId"), true);	// 患者唯一ID，必传，定位该患者所有就诊
	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("common.keyword"), false);			// 搜索关键词，可选，模糊匹配诊断、医生
	API_DTO_FIELD_REQUIRE(String, visitType, ZH_WORDS_GETTER("historyOrder.field.visitType"), false);	// 就诊类型筛选，可选
};

#include OATPP_CODEGEN_END(DTO)			// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_LIST_QUERY_H_	// 头文件保护宏结束
