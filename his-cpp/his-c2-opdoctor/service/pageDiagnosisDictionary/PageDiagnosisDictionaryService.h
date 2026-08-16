#pragma once
#ifndef PAGE_DIAGNOSISDICTIONARYSERVICE_H
#define PAGE_DIAGNOSISDICTIONARYSERVICE_H

#include "domain/do/pageDiagnosisDictionary/ClnConditionDefinitionDO.h"
#include "domain/query/pageDiagnosisDictionary/PageDiagnosisDictionaryQuery.h"
#include "domain/dto/pageDiagnosisDictionary/PageDiagnosisDictionaryDTO.h"
#include "dao/pageDiagnosisDictionary/PageDiagnosisDictionaryDAO.h"
#include "RedisClient.h"

class PageDiagnosisDictionaryService
{
public:
	// 返回完整的分页 DTO（PageDiagnosisDictionaryPageDTO::Wrapper）
	PageDiagnosisDictionaryPageDTO::Wrapper getPageDiagnosisDictionaryList(
		const PageDiagnosisDictionaryQuery::Wrapper& query,
		const std::string& tenantId,
		const std::string& practitionerId);

private:
	PageDiagnosisDictionaryDAO dao;

};



#endif // !1


