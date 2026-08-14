#pragma once
#ifndef _RENT_SERVICE_
#define _RENT_SERVICE_

#include "domain/dto/rent/RentDetailDTO.h"
#include "domain/dto/rent/RentNameDTO.h"
#include "domain/query/rent/RentQuery.h"

class RentService {
public:
	// 分页查询所有租户数据
	RentDetailPageDTO::Wrapper rentPage(const RentQuery::Wrapper& query);
	// 查询所有租户名称
	RentNamePageDTO::Wrapper findAllRent();
};


#endif // !_RENT_SERVICE_

