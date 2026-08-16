#include "stdafx.h"
#include "service/settings/SettingsService.h"
#include "dao/settings/SettingsDAO.h"

SettingsPageDTO::Wrapper SettingsService::queryPage(const SettingsQuery::Wrapper& query)
{
	auto pages = SettingsPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	SettingsDAO dao;
	uint64_t total = dao.count(query);
	if (total <= 0)
	{
		return pages;
	}

	pages->total = total;
	pages->calcPages();
	auto result = dao.selectWithPage(query);

	for (auto& sub : result)
	{
		auto dto = SettingsDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		dto->value = sub.getValue();
		dto->description = sub.getDescription();
		dto->provider_name = sub.getProviderName();
		dto->provider_key = sub.getProviderKey();
		dto->tenant_id = sub.getTenantId();
		pages->addData(dto);
	}
	return pages;
}

oatpp::List<SettingsDTO::Wrapper> SettingsService::queryByName(const ConfigItemQuery::Wrapper& query)
{
	SettingsDAO dao;
	auto result = dao.selectByName(query->name.getValue(""));

	auto list = oatpp::List<SettingsDTO::Wrapper>::createShared();
	for (auto& sub : result)
	{
		auto dto = SettingsDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		dto->value = sub.getValue();
		dto->description = sub.getDescription();
		dto->provider_name = sub.getProviderName();
		dto->provider_key = sub.getProviderKey();
		dto->tenant_id = sub.getTenantId();
		list->push_back(dto);
	}
	return list;
}