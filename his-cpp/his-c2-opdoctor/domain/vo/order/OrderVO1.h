#pragma once
#ifndef _ORDER_VO_H_
#define _ORDER_VO_H_

#include "../../dto/order/OrderDetailDTO1.h"
#include "../../dto/order/CatalogItemDTO1.h"


#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section

/** 
* 定义一个分页查询结果的JsonVO对象，用于返回给前端(医嘱详情分页查询)
*/
class OrderDetailPageJsonVO1 : public JsonVO<OrderDetailPageDTO1::Wrapper> {
	DTO_INIT(OrderDetailPageJsonVO1, JsonVO<OrderDetailPageDTO1::Wrapper>);
};

/*
* 定义一个分页查询结果的JsonVO对象，用于返回给前端（药品/诊疗/耗材目录分页查询）
*/
class CatalogItemPageJsonVO1 : public JsonVO<CatalogItemPageDTO1::Wrapper> {
	DTO_INIT(CatalogItemPageJsonVO1, JsonVO<CatalogItemPageDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif