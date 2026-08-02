#pragma once
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_

#include <string>
#include <unordered_map>
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/dto/PayloadDTO.h"
#include "domain/vo/ward/WardNameVO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "domain/query/ward/WardTreeQuery.h"

class WardService
{
public:
    oatpp::List<WardTreeNodeVO::Wrapper> GetTree(WardTreeQuery::Wrapper query);

    std::string createLocation(
        const WardLocationAddDTO::Wrapper& dto,
        const PayloadDTO& payload);
};

#endif
