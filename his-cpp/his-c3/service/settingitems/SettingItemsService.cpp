#include "stdafx.h"
#include "SettingItemsService.h"
#include "SimpleDateTimeFormat.h"
#include "id/UuidFacade.h"
#include "../../dao/settingitems/SettingItemsDAO.h"
#include "../../domain/do/settingitems/SettingItemsDO.h"

bool SettingItemsService::removeData(const std::string& id, const std::string& deletedBy)
{
	SettingItemsDAO dao;
	// Return true only when exactly 1 row is affected
	return dao.softDelete(id, deletedBy) == 1;
}

std::string SettingItemsService::addData(const SettingAddDTO::Wrapper& dto)
{
	SettingItemsDO data;
	// Map DTO fields to DO
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Name, name, Value, value,
		ProviderName, providerName, ProviderKey, providerKey,
		TenantId, tenantId, Description, description);

	// Generate primary key
	UuidFacade uf;
	data.setId(uf.genUuid());

	// Fill audit fields
	data.setCreatedBy(dto->getPayload()->getUsername());
	data.setCreatedAt(SimpleDateTimeFormat::format());
	data.setIsDeleted(0);

	// Insert and return new ID
	SettingItemsDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}

bool SettingItemsService::modifyData(const SettingModifyDTO::Wrapper& dto)
{
	SettingItemsDO data;
	// Map DTO fields to DO (including ID)
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Id, id, Name, name, Value, value,
		ProviderName, providerName, ProviderKey, providerKey,
		TenantId, tenantId, Description, description);

	// Fill audit fields
	data.setUpdatedBy(dto->getPayload()->getUsername());
	data.setUpdatedAt(SimpleDateTimeFormat::format());

	// Update and return success status
	SettingItemsDAO dao;
	return dao.update(data) == 1;
}
