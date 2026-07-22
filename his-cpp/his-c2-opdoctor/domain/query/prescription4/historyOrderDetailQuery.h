#pragma once
#ifndef _HISTORY_ORDER_DETAIL_QUERY_H_
#define _HISTORY_ORDER_DETAIL_QUERY_H_

#include "../../GlobalInclude.h"			// 项目全局公共头文件
#include "domain/query/PageQuery.h"			// 分页查询基类，继承分页参数

#include OATPP_CODEGEN_BEGIN(DTO)			// oatpp DTO 代码生成开始宏

/**
 * 历史医嘱明细列表查询请求
 * 场景：选中左侧就诊记录，加载右侧医嘱明细列表
 */
class HistoryOrderDetailQuery : public PageQuery {
	DTO_INIT(HistoryOrderDetailQuery, PageQuery); // DTO 初始化宏，声明父子类关系

	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("historyOrder.field.visitId"), true);	// 就诊记录唯一ID，必传，定位单次就诊医嘱
	API_DTO_FIELD_REQUIRE(String, orderType, ZH_WORDS_GETTER("historyOrder.field.orderType"), false);	// 医嘱类型筛选，可选，对应顶部下拉
	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("common.keyword"), false);			// 搜索关键词，可选，模糊匹配医嘱名称、诊断
};

#include OATPP_CODEGEN_END(DTO)			// oatpp DTO 代码生成结束宏
#endif // _HISTORY_ORDER_DETAIL_QUERY_H_	// 头文件保护宏结束
