#pragma once
#ifndef _CATALOG_QUERY_H_
#define _CATALOG_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 药品/诊疗/耗材目录分页查询请求
 * 场景：点开搜索框、切换Tab、输入关键词时调用
 */
class CatalogQuery1 : public PageQuery {
	DTO_INIT(CatalogQuery1, PageQuery);

	// === 业务上下文（必传，用于权限及库存过滤） ===
	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("prescription1.order.field.visitid"), true);		// 就诊ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("prescription1.order.field.patientid"), true);		// 患者ID
	API_DTO_FIELD_REQUIRE(String, departmentId, ZH_WORDS_GETTER("prescription1.order.field.departmentId"), true);		// 科室ID

	// === 搜索关键词（前端输入的内容） ===
	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("common.keyword"), false);

	// === Tab 筛选（全部/药品/诊疗/耗材）===
	API_DTO_FIELD_REQUIRE(List<String>, excludeMedicationCategoryCodes, ZH_WORDS_GETTER("prescription1.order.field.excludeCodes"), false);

	// === 是否仅显示有库存（默认 false） ===
	API_DTO_FIELD_REQUIRE(Boolean, strictStockCheck, ZH_WORDS_GETTER("prescription1.order.field.strictStock"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif
