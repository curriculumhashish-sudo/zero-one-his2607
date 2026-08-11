#include "stdafx.h"
#include "RentController.h"
#include "service/rent/RentService.h"

RentDetailPageJsonVO::Wrapper RentController::execRentPageQuery(const RentQuery::Wrapper& query)
{
	// 查询数据
	auto result = RentService().rentPage(query);
	// 响应结果
	auto jvo = RentDetailPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

RentNameJsonVO::Wrapper RentController::execRentNameQuery()
{
	// 查询数据
	auto result = RentService().findAllRent();
	// 响应结果
	auto jvo = RentNameJsonVO::createShared();
	jvo->success(result);
	return jvo;
}
