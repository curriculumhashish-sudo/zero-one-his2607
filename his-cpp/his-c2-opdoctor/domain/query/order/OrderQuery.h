#pragma once
#ifndef _ORDER_QUERY_H_
#define _ORDER_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section

/**
* 根据就诊ID和患者ID查询患者医嘱信息
* 演示项目中是通过患者队列中点击患者挂号卡片来传递就诊ID和患者ID的
*/
class OrderQuery : public PageQuery
{
	DTO_INIT(OrderQuery, PageQuery);
	// 就诊ID
	API_DTO_FIELD_REQUIRE(String, visitId, ZH_WORDS_GETTER("order.field.visitid"), true);
	// 患者ID
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("order.field.patientid"), true);
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif