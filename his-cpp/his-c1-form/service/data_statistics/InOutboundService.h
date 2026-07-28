#pragma once
#ifndef _INOUTBOUNDSERVICE_H_
#define _INOUTBOUNDSERVICE_H_

#include "domain/vo/data_statistics/InOutboundVo.h"
#include "domain/query/data_statistics/InOutboundQuery.h"
#include "domain/dto/data_statistics/InOutboundDto.h"

class InOutboundService {
public:
	InOutboundPageDto::Wrapper listAll(const InOutboundQuery::Wrapper& query);
	InOutboundDto::Wrapper getById(std::string id);
};

#endif // !_INOUTBOUNDSERVICE_H_
