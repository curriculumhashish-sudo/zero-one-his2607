#pragma once
#ifndef _CATALOG_QUERY_1_H_
#define _CATALOG_QUERY_1_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 药品/诊疗/耗材目录分页查询请求
 * 场景：点开搜索框、切换Tab、输入关键词时调用
 */
class CatalogQuery1 : public PageQuery {
    DTO_INIT(CatalogQuery1, PageQuery);

    // === 搜索关键词（前端输入的内容） ===
    API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("common.keyword"), false);

    // === Tab 筛选（全部/药品/诊疗/耗材） ===
    // 可选值：ALL、MEDICATION、TREATMENT、DEVICE
    API_DTO_FIELD_REQUIRE(String, tab, ZH_WORDS_GETTER("createOrder.order.field.tab"), false);

    // === Tab 排除类别（保留兼容，但实际使用 tab 字段） ===
    API_DTO_FIELD_REQUIRE(List<String>, excludeMedicationCategoryCodes, ZH_WORDS_GETTER("createOrder.order.field.excludeCodes"), false);

    // === 是否仅显示有库存（默认 false，暂不实现库存过滤） ===
    API_DTO_FIELD_REQUIRE(Boolean, strictStockCheck, ZH_WORDS_GETTER("createOrder.order.field.strictStock"), false);
};

#include OATPP_CODEGEN_END(DTO)
#endif