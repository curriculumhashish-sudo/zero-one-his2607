#include "stdafx.h"
#include "EquipmentService.h"
#include "../../dao/equipment/EquipmentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

// 列表分页
EquipmentDetailPageDTO::Wrapper EquipmentService::listAll(const EquipmentQuery::Wrapper& query)
{
	// 构建返回对象
	auto pages = EquipmentDetailPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	EquipmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<DocInventoryItemSnapshotDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (DocInventoryItemSnapshotDO& sub : result)
	{
		auto dto = EquipmentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, 
			// id
			id, Id,
			// 编码
			code,ProductBusNo,
			// 项目名
			name, Name,
			// 规格
			specification, TotalVolume,
			// 状态
			status, InventoryStatusEnum,
			// 分类
			classification, DeviceCategoryCode,
			// 厂家
			manufacturer, Manufacturer,
			// 价格
			price, LotNumberPriceAmount
		);
		pages->addData(dto);
	}
	return pages;
}

// 通过id查询
EquipmentDTO::Wrapper EquipmentService::getById(std::string id)
{
	// 查询数据
	EquipmentDAO dao;
	auto res = dao.selectById(id);

	// 没有查询到数据
	if (!res)
		return nullptr;

	// 查询到数据转换成DTO
	auto dto = EquipmentDTO::createShared();
	ZO_STAR_DOMAIN_DO_TO_DTO_1(
		dto, res,
		// 基础主键
		id, Id,
		// 器材名称
		name, Name,
		// 商品名称
		goods, MerchandiseName,
		// 器材分类编码
		classification, CategoryCode,
		// 生产厂家名称
		manufacturer, ManufacturerText,
		// 供应商标识
		supplier, SupplyId,
		// 批准文号
		apprno, ApprovalNumber,
		// 包装规格描述
		specification, TotalVolume,
		// 医保编码
		code, YbNo,
		// 财务类型：DO无对应字段
		// finantype, 无对应DO字段,
		// 院内业务编码
		code, BusNo,
		// 医保类别：DO无此字段
		// medinscat, 无对应DO字段,
		// 医保等级
		medinsgra, ChrgitmLv,
		// 采购单位编码
		purchaunit, PurchaseUnitCode,
		// 采购/拆零倍数（采购换算比例）
		purmin, PurchaseUnitRatio,
		// 购入价（整包装价格）
		purchase, LotNumberCostAmount,
		// 库存单位编码
		inventory, SplitUnitCode,
		// 库存拆零倍数（库存换算比例）
		invsplit, StockUnitRatio,
		// 零售价（整包装售价）
		retail, LotNumberPriceAmount,
		// 包装单位编码
		packaging, PurchaseUnitCode,
		// 包装拆零倍数（包装换算比例）
		packsplit, PackageUnitRatio,
		// 拆零单位编码
		split, SplitUnitCode,
		// 最高零售价：DO无对应字段
		// maxretail, 无对应DO字段,
		// 库存警戒上限
		warningmax, ItemMaxQuantity,
		// 库存警戒下限
		warningmin, ItemMinQuantity,
		// 拼音检索码
		pinyin, PyStr,
		// 五笔检索码
		wubi, WbStr
	);
	return dto;
}

// 新增器材
std::string EquipmentService::saveData(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	WkfInventoryItemDO entity;

	UuidFacade uuidFacade;
	std::string newId = uuidFacade.genUuid();
	entity.setId(newId);


	entity.setName(dto->name.getValue(""));
	entity.setCategoryCode(dto->classification.getValue(""));

	//给默认值防止报错
	entity.setItemTable("wkf_equipment");     // 必填：物品表名
	entity.setItemId("test_item_id");         // 必填：物品定义标识
	entity.setSupplierId("default_supplier"); // 必填：供应商标识
	entity.setUnitCode("box");                // 必填：单位编码
	entity.setQuantity(1.0);                  // 必填：数量
	entity.setLotNumber("LOT000000");         // 必填：批号
	entity.setLocationId("loc_01");           // 必填：库房标识

	entity.setCreatedBy(payload.getId());
	entity.setCreatedAt(SimpleDateTimeFormat::format());

	// 假设默认启用
	entity.setInventoryStatusEnum("ACTIVE");

	if (dao.insert(entity) > 0) {
		return newId;
	}
	return "";
}

// 修改器材
bool EquipmentService::modifyData(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	WkfInventoryItemDO entity;

	// 使用 .getValue 获取值
	entity.setId(dto->id.getValue(""));
	entity.setName(dto->name.getValue(""));

	// 状态转换：前端传1(启用)，数据库存 "ACTIVE"
	if (dto->status.getValue(1) == 1) {
		entity.setInventoryStatusEnum("ACTIVE");
	}
	else {
		entity.setInventoryStatusEnum("INACTIVE");
	}

	entity.setUpdatedBy(payload.getId());
	entity.setUpdatedAt(SimpleDateTimeFormat::format());

	return dao.update(entity) > 0;
}

// 批量删除器材 (软删除)
bool EquipmentService::deleteData(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;

	if (!dto->ids || dto->ids->empty()) {
		return false;
	}

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		// idItem 也是 oatpp::String，取值用 .getValue
		idList.push_back(idItem.getValue(""));
	}

	std::string deletedBy = payload.getId();
	std::string deletedAt = SimpleDateTimeFormat::format();

	return dao.softDeleteByIds(idList, deletedBy, deletedAt) > 0;
}

// 批量启停用器材
bool EquipmentService::changeStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (!dto->ids || dto->ids->empty()) {
		return false;
	}

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		idList.push_back(idItem.getValue(""));
	}

	// 状态转换
	std::string targetStatus = (dto->status.getValue(1) == 1) ? "ACTIVE" : "INACTIVE";
	std::string updatedBy = payload.getId();
	std::string updatedAt = SimpleDateTimeFormat::format();

	return dao.updateStatusByIds(idList, targetStatus, updatedBy, updatedAt) > 0;
}

// 选择器材 (下拉框精简版)
EquipmentSelectPageDTO::Wrapper EquipmentService::selectData(const EquipmentQuery::Wrapper& query)
{
	auto pages = EquipmentSelectPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	EquipmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) return pages;

	pages->total = count;
	pages->calcPages();

	list<DocInventoryItemSnapshotDO> result = dao.selectWithPage(query);
	for (DocInventoryItemSnapshotDO& sub : result)
	{
		auto dto = EquipmentSelectDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		// dto->specification = sub.getSpecification();
		pages->addData(dto);
	}
	return pages;
}