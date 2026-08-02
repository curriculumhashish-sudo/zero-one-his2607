#include "stdafx.h"
#include "Patcontroller.h"
#include "../../service/pat/PatService.h"

PatPageJsonVO::Wrapper PatController::execQueryPatPage(const PatPageQuery::Wrapper& query, const PayloadDTO& payload)
{
	// Normalize paging defaults for management list (UI default 10 / page)
	if (!query->pageIndex || query->pageIndex < 1)
	{
		query->pageIndex = 1;
	}
	if (!query->pageSize || query->pageSize < 1)
	{
		query->pageSize = 10;
	}

	PatService patService;
	auto pageData = patService.queryPatPage(query);

	auto respVo = PatPageJsonVO::createShared();
	respVo->success(pageData);
	respVo->message = ZH_WORDS_GETTER("pat.page.success");
	return respVo;
}

PatSelectPageJsonVO::Wrapper PatController::execQuerySelectPatPage(const PatSelectPageQuery::Wrapper& query, const PayloadDTO& payload)
{
	// Normalize paging defaults for popup (UI default 20 / page)
	if (!query->pageIndex || query->pageIndex < 1)
	{
		query->pageIndex = 1;
	}
	if (!query->pageSize || query->pageSize < 1)
	{
		query->pageSize = 20;
	}

	PatService patService;
	auto pageData = patService.querySelectPatPage(query);

	auto respVo = PatSelectPageJsonVO::createShared();
	respVo->success(pageData);
	respVo->message = ZH_WORDS_GETTER("pat.select.page.success");
	return respVo;
}

ListJsonVO<String>::Wrapper PatController::execRemovePat(const List<String>& ids)
{
	auto jvo = ListJsonVO<String>::createShared();
	if (!ids || ids->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	return jvo;
}