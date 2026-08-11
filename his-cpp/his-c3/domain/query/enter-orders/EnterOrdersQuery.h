#pragma once
#ifndef _ENTERORDERSQUERY_H_
#define _ENTERORDERSQUERY_H_
#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
//EnterOrders
//enter-orders
//ENTERORDERS
#include OATPP_CODEGEN_BEGIN(DTO)

//通过患者id查询
class EnterOrdersQueryById :public PageQuery
{
	//定义
	DTO_INIT(EnterOrdersQueryById, DTO);
	//租户id
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("enter-orders.tanent_id"), true);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("enter-orders.patient_id"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif
