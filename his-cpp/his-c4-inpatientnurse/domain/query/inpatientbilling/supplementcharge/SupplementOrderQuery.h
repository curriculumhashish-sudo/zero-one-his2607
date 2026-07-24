#pragma once
//获取患者医嘱补费 条件+分页
#ifndef _SUPPLEMENTORDERQUERY_H_
#define _SUPPLEMENTORDERQUERY_H_

#include "../../../GlobalInclude.h"
#include "ApiHelper.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupplementOrderQuery : public PageQuery {

	DTO_INIT(SupplementOrderQuery, DTO);

	//类型 全部 长期 临时
	API_DTO_FIELD_REQUIRE(String, type, ZH_WORDS_GETTER("supplementorder.field.type"), true);

	//医嘱名称
	API_DTO_FIELD_REQUIRE(String, orderName, ZH_WORDS_GETTER("supplementorder.field.orderName"), false);

	//开方科室
	API_DTO_FIELD_REQUIRE(String, openDeptCode, ZH_WORDS_GETTER("supplementorder.field.openDeptCode"), false);

};


#include OATPP_CODEGEN_END(DTO)



#endif //_SUPPLEMENTORDERQUERY_H_