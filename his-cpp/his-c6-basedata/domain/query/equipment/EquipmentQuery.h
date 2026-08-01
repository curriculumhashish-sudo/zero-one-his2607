#pragma once

#ifndef _EQUIPMENTQUERY_H_
#define _EQUIPMENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EquipmentQuery : public PageQuery
{
	DTO_INIT(EquipmentQuery, PageQuery);
	
	// 关键词
	API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("equipment.field.keywords"), false);
	
	// 器材分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("equipment.field.classification"), false);

	// 启停状态过滤 (1启用 0停用，false表示不是必传项)
	API_DTO_FIELD_REQUIRE(Int32, activeFlag, ZH_WORDS_GETTER("equipment.field.activeFlag"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_EQUIPMENTQUERY_H_