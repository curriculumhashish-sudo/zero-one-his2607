#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_SERVICE_
#define _PRINT_JOB_SERVICE_

#include "../../domain/dto/opnurse3/PrintJobDTO.h"
#include "../../domain/query/opnurse3/PrintJobQuery.h"
#include "../../dao/opnurse3/PrintJobDAO.h"

class PrintJobService
{
public:
	PrintJobPageDTO::Wrapper listAll(const PrintJobQuery::Wrapper& query);
};

#endif // !_PRINT_JOB_SERVICE_
