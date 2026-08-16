#pragma once
#ifndef _ENTERORDERSVO_H_
#define _ENTERORDERSVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/enter-orders/EnterOrdersDTO.h"
//EnterOrders
//enter-orders
//ENTERORDERS
#include OATPP_CODEGEN_BEGIN(DTO)

///**
// * 示例显示JsonVO，用于响应给客户端的Json对象
// */
//class SampleJsonVO : public JsonVO<SampleDTO::Wrapper> {
//	DTO_INIT(SampleJsonVO, JsonVO<SampleDTO::Wrapper>);
//};

/**
 * 示例分页显示JsonVO，用于响应给客户端的Json对象
 */

class QueryPageJsonVO : public JsonVO<EnterOrdersQueryPageDTO::Wrapper> {
	DTO_INIT(QueryPageJsonVO, JsonVO<EnterOrdersQueryPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_