#pragma once
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_
#include "../include/id/SnowFlake.h"
#include <string>
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "domain/dto/ward/WardLocationAddDTO.h"
#include "domain/dto/PayloadDTO.h"
#include "../../domain/vo/ward/WardNameVO.h"
#include "../../domain/vo/ward/WardTreeNodeVO.h"
#include"domain/query/ward/WardTreeQuery.h"
#include<unordered_map>
class WardService
{
public:
    oatpp::List<WardTreeNodeVO::Wrapper>WardService::GetTree(WardTreeQuery::Wrapper query);
    //bool pruneByStatus(
    //    const WardTreeNodeVO::Wrapper& node,
    //    const std::string& statusEnum);

    //bool pruneByKeyword(
    //    const WardTreeNodeVO::Wrapper& node,
    //    const std::string& keyword);
        std::string WardService::createLocation(
            const WardLocationAddDTO::Wrapper& dto,
            const PayloadDTO& payload);
   
};

#endif
