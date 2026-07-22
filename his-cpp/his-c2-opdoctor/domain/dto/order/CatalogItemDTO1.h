#pragma once
#ifndef _CATALOG_ITEM_DTO_H_
#define _CATALOG_ITEM_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 目录搜索下拉列表项（也就是药品/诊疗/耗材的基本信息，提供给医生来开具医嘱）
 */
class CatalogItemDTO1 : public oatpp::DTO
{
	DTO_INIT(CatalogItemDTO1, DTO);

	API_DTO_FIELD_REQUIRE(String, itemCode, ZH_WORDS_GETTER("prescription1.order.field.itemCode"), true);    // 项目编码（主键）
	API_DTO_FIELD_REQUIRE(String, itemName, ZH_WORDS_GETTER("prescription1.order.field.itemName"), true);    // 项目名称
	API_DTO_FIELD_REQUIRE(String, category, ZH_WORDS_GETTER("prescription1.order.field.category"), true);    // 类别（药品/诊疗/耗材）
	API_DTO_FIELD_REQUIRE(String, spec, ZH_WORDS_GETTER("prescription1.order.field.spec"), true);            // 规格
	API_DTO_FIELD_REQUIRE(Int32, stockQuantity, ZH_WORDS_GETTER("prescription1.order.field.stock"), true);   // 库存数量
	API_DTO_FIELD_REQUIRE(Float64, unitPrice, ZH_WORDS_GETTER("prescription1.order.field.unitPrice"), true); // 单价

};

/*
* 分页展示
*/
class CatalogItemPageDTO1 : public PageDTO<CatalogItemDTO1::Wrapper> {
	DTO_INIT(CatalogItemPageDTO1, PageDTO<CatalogItemDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif