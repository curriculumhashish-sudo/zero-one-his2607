#pragma once
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_
#include<string>
#include "../../domain/vo/ward/WardNameVO.h"
#include "domain/vo/ward/WardItemVO.h"
#include"domain/query/ward/WardListQuery.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/do/ward/WardLocationDO.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "domain/dto/PayloadDTO.h"
class WardService
{
public:
	oatpp::List<WardItemVO::Wrapper> listWards(
		const WardListQuery::Wrapper& query);
	oatpp::List<WardNameVO::Wrapper>listNames(
		const std::string& organizationId

	);
	std::string addLocation(
		const WardLocationAddDTO::Wrapper& dto,
		const PayloadDTO& payload);



};

#endif // !
