#include "stdafx.h"
#include "PatController.h"
#include "StringUtil.h"
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

// ============================================================
// 批量删除患者（软删除）
// 调用链路: PatController → PatService::removePatient → PatientDAO::softDeletePatient
// 业务逻辑:
//   ① 校验ID列表非空，否则返回RS_PARAMS_INVALID(9995)
//   ② 遍历每个ID：trim + 跳过空值 + 长度校验(≤64)
//   ③ 依次调用Service软删除，删除成功则收集到deletedList
//   ④ 返回成功删除的ID列表
// 注意: 单个ID删除失败不影响其他ID，静默跳过
// ============================================================
ListJsonVO<String>::Wrapper PatController::execRemovePat(const List<String>& ids, const PayloadDTO& payload)
{
	auto jvo = ListJsonVO<String>::createShared();
	if (!ids || ids->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	PatService patService;
	auto deletedList = oatpp::List<String>::createShared();
	for (const auto& id : *ids)
	{
		if (!id || id->empty()) continue;
		std::string rawId = id;
		StringUtil::trim(&rawId);
		if (rawId.empty() || rawId.size() > 64) continue;  // 跳过非法ID
		if (patService.removePatient(rawId, payload))
		{
			deletedList->push_back(rawId);
		}
	}
	jvo->success(deletedList);
	return jvo;
}