#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_LOG_SERVICE_
#define _PRINT_LOG_SERVICE_

#include "../../domain/dto/opnurse3/PrintLogDTO.h"
#include "../../domain/query/opnurse3/PrintLogQuery.h"
#include "../../dao/opnurse3/PrintLogDAO.h"

class PrintLogService
{
public:
	PrintLogPageDTO::Wrapper listAll(const PrintLogQuery::Wrapper& query);
};

#endif // !_PRINT_LOG_SERVICE_
