#pragma once
#ifndef PageDiagnosisDictionaryDAO_H_
#define PageDiagnosisDictionaryDAO_H_

#include "BaseDAO.h"
#include "domain/do/pageDiagnosisDictionary/ClnConditionDefinitionDO.h"
#include "PageDiagnosisDictionaryMapper.h"
#include "domain/query/pageDiagnosisDictionary/PageDiagnosisDictionaryQuery.h"
#include <list>

class PageDiagnosisDictionaryDAO : public BaseDAO
{
public:
		std::list<PtrClnConditionDefinitionDO> queryDiagnosisDictionary(
		const PageDiagnosisDictionaryQuery::Wrapper& query,
		const std::string& tenantId,
		const std::string& practitionerId
	);


};




#endif // !
