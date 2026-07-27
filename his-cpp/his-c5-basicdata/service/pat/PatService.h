#pragma once
#ifndef _PAT_SERVICE_H_
#define _PAT_SERVICE_H_

#include "../../domain/query/pat/PatSelectPageQuery.h"
#include "../../domain/dto/pat/PatSelectDTO.h"


class PatService
{
public:
    // 分页查询选择患者列表
    PageDTO<PatSelectDTO>::Wrapper querySelectPatPage(const PatSelectPageQuery::Wrapper& query);
};

#endif
