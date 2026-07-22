#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_DAO_
#define _PRINT_JOB_DAO_
#include "BaseDAO.h"
#include "../../domain/do/opnurse3/PrintJobDO.h"
#include "../../domain/query/opnurse3/PrintJobQuery.h"

class PrintJobDAO : public BaseDAO
{
private:
	inline std::string queryConditionBuilder(const PrintJobQuery::Wrapper& query, SqlParams& params);
public:
	uint64_t count(const PrintJobQuery::Wrapper& query);
	std::list<PrintJobDO> selectWithPage(const PrintJobQuery::Wrapper& query);
	PtrPrintJobDO selectById(std::string id);
};
#endif // !_PRINT_JOB_DAO_
