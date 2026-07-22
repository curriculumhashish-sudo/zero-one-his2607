#pragma once
#ifndef _MATERIAL_QUERY_H_
#define _MATERIAL_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class MaterialQuery : public PageQuery
{
	DTO_INIT(MaterialQuery, PageQuery);
	// 精确搜项目编码
	API_DTO_FIELD_REQUIRE(String, materialCode, ZH_WORDS_GETTER("material.field.materialCode"), false);
	// 模糊搜药材名称
	API_DTO_FIELD_REQUIRE(String, materialName, ZH_WORDS_GETTER("material.field.materialName"), false);
	// 类别筛选：中草药、耗材等
	API_DTO_FIELD_REQUIRE(String, category, ZH_WORDS_GETTER("material.field.category"), false);
};
#include OATPP_CODEGEN_END(DTO)
#endif