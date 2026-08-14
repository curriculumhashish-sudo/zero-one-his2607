#include "stdafx.h"
#include "TenantUserService.h"
#include "dao/tenantuser/TenantUserDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include <set>

namespace
{
	std::list<std::string> normalizeUserIds(const oatpp::List<oatpp::String>& userIds)
	{
		std::list<std::string> results;
		std::set<std::string> exists;
		for (auto userId : *userIds.get())
		{
			std::string value = userId.getValue("");
			if (!value.empty() && exists.insert(value).second)
				results.push_back(value);
		}
		return results;
	}

	oatpp::List<oatpp::String> toOatppList(const std::list<std::string>& values)
	{
		auto list = oatpp::List<oatpp::String>::createShared();
		for (const auto& value : values)
			list->push_back(value.c_str());
		return list;
	}
}

TenantUserPageDTO::Wrapper TenantUserService::listAll(const TenantUserQuery::Wrapper& query)
{
	auto page = TenantUserPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;

	TenantUserDAO dao;
	auto total = dao.count(query);
	if (total <= 0)
		return page;

	page->total = total;
	page->calcPages();
	auto rows = dao.selectPage(query);
	for (auto& row : rows)
	{
		auto dto = TenantUserDTO::createShared();
		dto->id = row->getId().c_str();
		dto->tenantId = row->getTenantId().c_str();
		dto->userId = row->getUserId().c_str();
		dto->userName = row->getUserName().c_str();
		dto->name = row->getName().c_str();
		dto->phoneNumber = row->getPhoneNumber().c_str();
		dto->email = row->getEmail().c_str();
		dto->isActive = row->getIsActive();
		dto->createdAt = row->getCreatedAt().c_str();
		dto->createdBy = row->getCreatedBy().c_str();
		page->addData(dto);
	}
	return page;
}

TenantUserBatchResult TenantUserService::saveBatch(const TenantUserBatchAddDTO::Wrapper& dto)
{
	TenantUserBatchResult result;
	TenantUserDAO dao;
	auto inputUserIds = normalizeUserIds(dto->userIds);
	auto validUserIds = dao.selectExistingUserIds(inputUserIds);
	if (validUserIds.empty())
	{
		result.success = true;
		return result;
	}

	auto boundUserIds = dao.selectBoundUserIds(dto->tenantId.getValue(""), validUserIds);
	std::set<std::string> boundSet(boundUserIds.begin(), boundUserIds.end());

	std::list<TenantUserRelDO> relations;
	UuidFacade uuid(1);
	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";
	for (const auto& userId : validUserIds)
	{
		if (boundSet.find(userId) != boundSet.end())
			continue;
		TenantUserRelDO& relation = relations.emplace_back();
		relation.setId(uuid.genUuid());
		relation.setTenantId(dto->tenantId.getValue(""));
		relation.setUserId(userId);
		relation.setCreatedAt(now);
		relation.setUpdatedAt(now);
		relation.setCreatedBy(username);
		relation.setUpdatedBy(username);
		relation.setIsDeleted(0);
		result.userIds->push_back(userId.c_str());
	}

	if (relations.empty())
	{
		result.success = true;
		return result;
	}

	result.success = dao.insertBatch(relations) == static_cast<int>(relations.size());
	if (!result.success)
		result.userIds = oatpp::List<oatpp::String>::createShared();
	return result;
}

TenantUserBatchResult TenantUserService::removeBatch(const TenantUserBatchRemoveDTO::Wrapper& dto)
{
	TenantUserBatchResult result;
	TenantUserDAO dao;
	auto inputUserIds = normalizeUserIds(dto->userIds);
	auto boundUserIds = dao.selectBoundUserIds(dto->tenantId.getValue(""), inputUserIds);
	if (boundUserIds.empty())
	{
		result.success = true;
		return result;
	}

	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";
	int rows = dao.softDeleteByTenantAndUserIds(dto->tenantId.getValue(""), boundUserIds, username, now);
	result.success = rows == static_cast<int>(boundUserIds.size());
	if (result.success)
		result.userIds = toOatppList(boundUserIds);
	return result;
}
