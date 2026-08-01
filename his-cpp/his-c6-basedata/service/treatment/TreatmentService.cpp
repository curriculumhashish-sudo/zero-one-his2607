/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:13:11

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "TreatmentService.h"
#include "../../dao/common/TreatmentEnumDictionaryDAO.h"
#include "../../dao/treatment/TreatmentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <stdexcept>

/** 将数据库状态枚举字符串转换为接口状态数值。 */
static int treatmentStatusValue(const std::string& status)
{
	if (status == "DRAFT")
		return 1;
	if (status == "ACTIVE")
		return 2;
	if (status == "INACTIVE")
		return 3;
	if (status == "DISCARDED")
		return 4;
	return 0;
}

// 从新增/修改 DTO 填充主表业务字段（不含审计字段）。
template<typename TDto>
static void fillMainFromAddDto(TreatmentDO& data, const TDto& dto)
{
	if (dto->name) data.setName(dto->name.getValue(""));
	if (dto->categoryCode) data.setCategoryCode(dto->categoryCode.getValue(""));
	if (dto->typeEnum) data.setTypeEnum(std::to_string(dto->typeEnum.getValue(0)));
	if (dto->packageUnitCode) data.setPackageUnitCode(dto->packageUnitCode.getValue(""));
	if (dto->pyStr) data.setPyStr(dto->pyStr.getValue(""));
	if (dto->wbStr) data.setWbStr(dto->wbStr.getValue(""));
	if (dto->descriptionText) data.setDescriptionText(dto->descriptionText.getValue(""));
	if (dto->ybFlag) data.setYbFlag(dto->ybFlag.getValue(0));
	if (dto->ybNo) data.setYbNo(dto->ybNo.getValue(""));
	if (dto->ybMatchFlag) data.setYbMatchFlag(dto->ybMatchFlag.getValue(0));
	if (dto->chrgitmLv) data.setChrgitmLv(dto->chrgitmLv.getValue(0));

	bool hasChildren = dto->children && dto->children->size() > 0;
	data.setChildrenFlag(hasChildren ? 1 : 0);
}

// 插入诊疗子项列表。
static bool insertChildren(TreatmentDAO& dao, const std::string& activityId,
	const oatpp::List<TreatmentChildDTO::Wrapper>& children,
	const std::string& username, const std::string& now)
{
	if (!children || children->size() == 0)
		return true;

	UuidFacade uf;
	for (auto childDto : *children.get())
	{
		TreatmentChildDO child;
		child.setId(uf.genUuid());
		child.setActivityId(activityId);
		if (childDto->childrenId) child.setChildrenId(childDto->childrenId.getValue(""));
		if (childDto->childrenName) child.setChildrenName(childDto->childrenName.getValue(""));
		if (childDto->childrenNum) child.setChildrenNum(childDto->childrenNum.getValue(0));
		child.setCreatedBy(username);
		child.setCreatedAt(now);
		child.setIsDeleted(0);
		child.setVersion(0);
		if (dao.insert(child) != 1)
			return false;
	}
	return true;
}

// 将主表 DO 转换为详情 DTO（不含子项/价格）。
static TreatmentDetailDTO::Wrapper toDetailDto(const TreatmentDO& src)
{
	auto dto = TreatmentDetailDTO::createShared();
	dto->id = src.getId();
	dto->code = src.getBusNo();
	dto->name = src.getName();
	dto->status = src.getStatusEnum();
	dto->categoryCode = src.getCategoryCode();
	dto->packageUnitCode = src.getPackageUnitCode();
	dto->pyStr = src.getPyStr();
	dto->wbStr = src.getWbStr();
	dto->descriptionText = src.getDescriptionText();
	dto->ybFlag = src.getYbFlag();
	dto->ybNo = src.getYbNo();
	dto->ybMatchFlag = src.getYbMatchFlag();
	dto->chrgitmLv = src.getChrgitmLv();
	// 仅在已赋值时拷贝（列表 join 可能有值）；详情价格由 fillPrices 填充。
	if (src.getRetailPricePtr())
		dto->retailPrice = src.getRetailPrice();

	const std::string& typeEnumStr = src.getTypeEnum();
	if (!typeEnumStr.empty())
	{
		try
		{
			dto->typeEnum = std::stoi(typeEnumStr);
		}
		catch (...)
		{
		}
	}
	return dto;
}

// 加载子项列表到详情 DTO。
static void fillChildren(TreatmentDetailDTO::Wrapper& dto, TreatmentDAO& dao, const std::string& activityId)
{
	auto childRows = dao.selectChildrenByActivityId(activityId);
	auto children = oatpp::List<TreatmentChildDTO::Wrapper>::createShared();
	for (auto& row : childRows)
	{
		auto childDto = TreatmentChildDTO::createShared();
		childDto->id = row.getId();
		childDto->childrenId = row.getChildrenId();
		childDto->childrenName = row.getChildrenName();
		childDto->childrenNum = row.getChildrenNum();
		children->push_back(childDto);
	}
	dto->children = children;
}

// 从 adm_charge_item_definition 填充成本价/售价/最高零售价。
static void fillPrices(TreatmentDetailDTO::Wrapper& dto, TreatmentDAO& dao, const std::string& activityId)
{
	auto rows = dao.selectPricesByInstanceId(activityId);
	for (auto& row : rows)
	{
		const std::string& priceType = row.getPriceTypeCode();
		if (priceType == "PURCHASE")
			dto->purchasePrice = row.getPriceValue();
		else if (priceType == "SALE")
			dto->retailPrice = row.getPriceValue();
		else if (priceType == "LIMIT")
			dto->maximumRetailPrice = row.getPriceValue();

		if (!row.getTypeCode().empty())
			dto->itemTypeCode = row.getTypeCode();
		if (!row.getYbType().empty())
			dto->ybType = row.getYbType();
	}
}

// 为诊疗插入 PURCHASE/SALE/LIMIT 价格行。
static bool insertPrices(TreatmentDAO& dao, const std::string& activityId,
	const TreatmentAddDTO::Wrapper& dto, const std::string& username, const std::string& now)
{
	struct PriceItem
	{
		const char* priceTypeCode;
		bool hasValue;
		double value;
	};

	PriceItem items[] = {
		{ "PURCHASE", (bool)dto->purchasePrice, dto->purchasePrice ? dto->purchasePrice.getValue(0) : 0 },
		{ "SALE", (bool)dto->retailPrice, dto->retailPrice ? dto->retailPrice.getValue(0) : 0 },
		{ "LIMIT", (bool)dto->maximumRetailPrice, dto->maximumRetailPrice ? dto->maximumRetailPrice.getValue(0) : 0 }
	};

	std::string typeCode = dto->itemTypeCode ? dto->itemTypeCode.getValue("1008") : "1008";
	if (typeCode.empty())
		typeCode = "1008";
	std::string ybType = dto->ybType ? dto->ybType.getValue("") : "";
	std::string chargeName = dto->name ? dto->name.getValue("") : "";

	UuidFacade uf;
	for (auto& item : items)
	{
		if (!item.hasValue)
			continue;

		AdmChargeItemDefinitionDO price;
		price.setId(uf.genUuid());
		price.setInstanceTable("wkf_activity_definition");
		price.setInstanceId(activityId);
		price.setChargeName(chargeName);
		price.setStatusEnum("ACTIVE");
		price.setTypeCode(typeCode);
		price.setYbType(ybType);
		price.setPriceTypeCode(item.priceTypeCode);
		price.setPriceValue(item.value);
		price.setCreatedBy(username);
		price.setCreatedAt(now);
		price.setIsDeleted(0);
		price.setVersion(0);
		if (dao.insert(price) != 1)
			return false;
	}
	return true;
}

TreatmentDetailDTO::Wrapper TreatmentService::getById(std::string id)
{
	// 查询主表
	TreatmentDAO dao;
	auto res = dao.selectById(id);

	// 没有查询到数据
	if (!res)
		return nullptr;

	// 查询到则转换成详情 DTO，并加载子项与价格
	auto dto = toDetailDto(*res);
	fillChildren(dto, dao, res->getId());
	fillPrices(dto, dao, res->getId());
	return dto;
}

bool TreatmentService::modifyStatus(const std::string& id, int32_t targetStatus, const std::string& operatorId, std::string& errorMsg) {
	std::string statusStr = std::to_string(targetStatus);
	auto record = TreatmentDAO().selectById(id);
	if (!record) {
		errorMsg = "record is not existed or deleted";
		return false;
	}
	if (record->getStatusEnum() == statusStr) {
		errorMsg = (targetStatus == 1) ?
			"enable status" : "stop status";
		return false;
	}
	int32_t rows = TreatmentDAO().updateStatusById(id, statusStr, operatorId, record->getVersion());
	if (rows == 0) {
		errorMsg = "update failed,the data may be moidfied by others,please reflush and retry";
		return false;
	}
	return true;
}

std::string TreatmentService::saveData(const TreatmentAddDTO::Wrapper& dto)
{
	TreatmentDO data;
	UuidFacade uf;
	std::string id = uf.genUuid();
	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";

	data.setId(id);
	// 业务编码暂用 id；有生成器后再替换。
	data.setBusNo(id);
	data.setStatusEnum("ACTIVE");
	data.setIsDeleted(0);
	data.setVersion(0);
	// 设置创建人
	data.setCreatedBy(username);
	// 设置创建时间
	data.setCreatedAt(now);
	fillMainFromAddDto(data, dto);

	// 事务内新增：主表 + 子项 + 价格。
	TreatmentDAO dao;
	TransactionManager tm(&dao);
	if (dao.insert(data) != 1)
	{
		tm.rollback();
		return "";
	}
	if (!insertChildren(dao, id, dto->children, username, now))
	{
		tm.rollback();
		return "";
	}
	if (!insertPrices(dao, id, dto, username, now))
	{
		tm.rollback();
		return "";
	}
	tm.commit();
	return id;
}

bool TreatmentService::saveDataBatch(const oatpp::List<TreatmentAddDTO::Wrapper>& datas)
{
	// 复用 saveData（主表+子项+价格）；全部成功才返回 true。
	for (auto dto : *datas.get())
	{
		if (saveData(dto).empty())
			return false;
	}
	return true;
}

bool TreatmentService::updateData(const TreatmentModifyDTO::Wrapper& dto)
{
	TreatmentDO data;
	std::string id = dto->id.getValue("");
	std::string now = SimpleDateTimeFormat::format();
	std::string username = dto->getPayload() ? dto->getPayload()->getUsername() : "";

	data.setId(id);
	// 设置修改人
	data.setUpdatedBy(username);
	// 设置修改时间
	data.setUpdatedAt(now);
	fillMainFromAddDto(data, dto);

	// 执行主表修改；子项/价格先软删再插入。
	TreatmentDAO dao;
	TransactionManager tm(&dao);
	if (dao.update(data) != 1)
	{
		tm.rollback();
		return false;
	}
	dao.softDeleteChildrenByActivityId(id, username, now);
	if (!insertChildren(dao, id, dto->children, username, now))
	{
		tm.rollback();
		return false;
	}
	dao.softDeletePricesByInstanceId(id, username, now);
	if (!insertPrices(dao, id, dto, username, now))
	{
		tm.rollback();
		return false;
	}
	tm.commit();
	return true;
}

TreatmentPageDTO::Wrapper TreatmentService::listAll(const TreatmentQuery::Wrapper& query)
{
	// 构建返回对象
	auto pages = TreatmentPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询总条数
	TreatmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<TreatmentDO> result = dao.selectWithPage(query);
	// 将 DO 转换成 DTO（列表不含子项）
	for (TreatmentDO& sub : result)
	{
		pages->addData(toDetailDto(sub));
	}
	return pages;
}

TreatmentSelectPageDTO::Wrapper TreatmentService::listSelect(
	const TreatmentSelectQuery::Wrapper& query,
	const PayloadDTO& payload)
{
	if (!query || !query->validate().empty() || payload.getId().empty())
	{
		return nullptr;
	}

	TreatmentDAO dao;
	// 按 JWT 用户解析出的有效租户过滤；查询始终按租户隔离。
	const std::string tenantId = dao.selectCurrentTenantId(payload.getId());
	if (tenantId.empty())
	{
		return nullptr;
	}

	// 初始化分页返回对象
	auto pages = TreatmentSelectPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	const uint64_t count = dao.countSelect(query, tenantId);
	pages->total = count;
	pages->calcPages();
	if (count == 0)
	{
		return pages;
	}

	// 将分页查询 BO 转换为接口 DTO
	std::list<TreatmentSelectBO> result =
		dao.selectSelectWithPage(query, tenantId);
	std::vector<std::string> statusCodes;
	for (const auto& sub : result)
	{
		statusCodes.emplace_back(sub.getStatusEnum());
	}
	// 状态显示文本统一从 MongoDB 枚举字典获取
	const auto statusDisplays = TreatmentEnumDictionaryDAO().findActiveDisplays(
		TreatmentEnumDictionaryDAO::PUBLICATION_STATUS_ENUM,
		statusCodes);
	for (TreatmentSelectBO& sub : result)
	{
		auto dto = TreatmentSelectDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(
			dto,
			sub,
			id, Id,
			busNo, BusNo,
			name, Name,
			categoryCode, CategoryCode,
			categoryCodeDictText, CategoryCodeDictText,
			orgIdDictText, OrgIdDictText,
			retailPrice, RetailPrice,
			specification, Specification,
			manufacturerText, ManufacturerText,
			itemTypeCode, ItemTypeCode,
			itemTypeCodeDictText, ItemTypeCodeDictText,
			packageUnitCode, PackageUnitCode,
			packageUnitCodeDictText, PackageUnitCodeDictText,
			defEncounterUnitCode, DefEncounterUnitCode,
			defEncounterUnitCodeDictText, DefEncounterUnitCodeDictText,
			ybNo, YbNo);
		const int status = treatmentStatusValue(sub.getStatusEnum());
		dto->statusEnum = status;
		const auto statusText = statusDisplays.find(sub.getStatusEnum());
		if (status == 0 || statusText == statusDisplays.end())
		{
			throw std::runtime_error("invalid treatment status enum");
		}
		dto->statusEnumText = statusText->second;
		pages->addData(dto);
	}
	return pages;
}

bool TreatmentService::removeData(const oatpp::List<oatpp::String>& ids)
{
	TreatmentDAO dao;
	std::list<std::string> datas;
	for (auto one : *ids.get())
		datas.emplace_back(one.getValue({}));
	int rows = dao.deleteByIds<TreatmentDO>(datas);
	if (rows == ids->size())
		return true;
	return false;
}
