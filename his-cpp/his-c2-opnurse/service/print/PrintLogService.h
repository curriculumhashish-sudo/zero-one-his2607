#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_LOG_SERVICE_
#define _PRINT_LOG_SERVICE_

#include "../../domain/dto/print/PrintLogDTO.h"
#include "../../domain/query/print/PrintLogQuery.h"
#include "../../dao/print/PrintLogDAO.h"

class PrintLogService
{
public:
	PrintLogPageDTO::Wrapper listAll(const PrintLogQuery::Wrapper& query);
};

#endif // !_PRINT_LOG_SERVICE_
