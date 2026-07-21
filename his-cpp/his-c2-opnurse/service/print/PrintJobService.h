#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_SERVICE_
#define _PRINT_JOB_SERVICE_

#include "../../domain/dto/print/PrintJobDTO.h"
#include "../../domain/query/print/PrintJobQuery.h"
#include "../../dao/print/PrintJobDAO.h"

class PrintJobService
{
public:
	PrintJobPageDTO::Wrapper listAll(const PrintJobQuery::Wrapper& query);
};

#endif // !_PRINT_JOB_SERVICE_
