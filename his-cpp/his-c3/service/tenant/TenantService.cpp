#include "stdafx.h"
#include "TenantService.h"
#include "../../dao/tenant/TenantDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

std::string TenantService::saveData(const TenantAddDTO::Wrapper& dto)
{
	//组装DO数据
	TenantDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Code, code, Name, name, IsActive, status, Description, description);
	// 设置ID
	UuidFacade uf;
	data.setId(uf.genUuid());
	// 设置创建人,创建时间
	data.setCreatedBy(dto->getPayload()->getUsername());
	data.setCreatedAt(SimpleDateTimeFormat::format());
	// 设置删除标记
	data.setIsDeleted(0);
	TenantDAO dao;
	return dao.insert(data) == 1 ? data.getId() : "";
}

bool TenantService::updateData(const TenantModifyDTO::Wrapper& dto)
{
	// 组装DO数据
	TenantDO data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Id, id, Code, code, Name, name, IsActive, status, Description, description);
	// 设置修改人
	data.setUpdatedBy(dto->getPayload()->getUsername());
	// 设置修改时间
	data.setUpdatedAt(SimpleDateTimeFormat::format());
	// 执行数据修改
	TenantDAO dao;
	return dao.update(data) == 1;
}

bool TenantService::removeData(const std::string& id, const std::string& deletedBy)
{
	TenantDAO dao;
	return dao.softDelete(id, deletedBy) == 1;
}
