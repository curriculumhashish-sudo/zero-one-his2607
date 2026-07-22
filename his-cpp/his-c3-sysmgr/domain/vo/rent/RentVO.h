#pragma once
#ifndef _Rent_VO_
#define _Rent_VO_

#include "domain/dto/rent/RentDetailDTO.h"
#include "domain/dto/rent/RentNameDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 租户分页显示JsonVO，用于响应给客户端的Json对象
 */
class RentDetailPageJsonVO : public JsonVO<RentDetailPageDTO::Wrapper> {
	DTO_INIT(RentDetailPageJsonVO, JsonVO<RentDetailPageDTO::Wrapper>);
};

/**
 * 示例显示JsonVO，用于响应给客户端的Json对象
 */
class RentNameJsonVO : public JsonVO<RentNameDTO::Wrapper> {
	DTO_INIT(RentNameJsonVO, JsonVO<RentNameDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _Rent_VO_