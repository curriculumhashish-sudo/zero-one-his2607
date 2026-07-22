#pragma once
#ifndef _ORDER_VO_H_
#define _ORDER_VO_H_

#include "../../dto/order/OrderDetailDTO.h"
#include "../../dto/order/CatalogItemDTO.h"


#include OATPP_CODEGEN_BEGIN(DTO) //<- Begin DTO codegen section

/** 
* 定义一个分页查询结果的JsonVO对象，用于返回给前端(医嘱详情分页查询)
*/
class OrderDetailPageJsonVO : public JsonVO<OrderDetailPageDTO::Wrapper> {
	DTO_INIT(OrderDetailPageJsonVO, JsonVO<OrderDetailPageDTO::Wrapper>);
};

/*
* 定义一个分页查询结果的JsonVO对象，用于返回给前端（药品/诊疗/耗材目录分页查询）
*/
class CatalogItemPageJsonVO : public JsonVO<CatalogItemPageDTO::Wrapper> {
	DTO_INIT(CatalogItemPageJsonVO, JsonVO<CatalogItemPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO) //<- End DTO codegen section
#endif