#include "stdafx.h"
#include "EquipmentService.h"
#include "../../dao/equipment/EquipmentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include <iomanip>
#include <sstream>


// 内部辅助函数：用于插入三个价格到 adm_charge_item_definition
static void _savePrices(EquipmentDAO& dao, const std::string& instanceId, const std::string& instanceName,
	const std::string& finantype, const std::string& ybType, const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	std::string currentTime = SimpleDateTimeFormat::format();
	std::string currentUserId = payload.getId(); // 当前操作人ID

	auto upsertPrice = [&](const std::string& typeCode, double priceVal, char prefix) {
		AdmChargeItemDefinitionDO pDo;

		std::string uniqueId = instanceId;
		if (!uniqueId.empty()) { uniqueId[0] = prefix; }
		pDo.setId(uniqueId);

		pDo.setTenantId("");
		pDo.setCreatedAt(currentTime);    // 仅 Insert 时生效
		pDo.setCreatedBy(currentUserId);  // 仅 Insert 时生效
		pDo.setInstanceTable("adm_device_definition");
		pDo.setInstanceId(instanceId);
		pDo.setChargeName(instanceName);
		pDo.setStatusEnum("ACTIVE");
		pDo.setTypeCode(finantype);
		pDo.setYbType(ybType);

		pDo.setPriceTypeCode(typeCode);
		pDo.setPriceValue(priceVal);

		try {
			// 调用支持审计追踪的 Upsert！
			dao.upsertChargeItem(pDo, currentTime, currentUserId);
		}
		catch (...) {}
		};

	// 记录本次需要保留的价格类型
	std::string pKeep = "NONE", sKeep = "NONE", lKeep = "NONE";

	if (dto->purchase) {
		upsertPrice("PURCHASE", dto->purchase.getValue(0.0), 'P');
		pKeep = "PURCHASE";
	}
	if (dto->retail) {
		upsertPrice("SALE", dto->retail.getValue(0.0), 'S');
		sKeep = "SALE";
	}
	if (dto->maxretail) {
		upsertPrice("LIMIT", dto->maxretail.getValue(0.0), 'L');
		lKeep = "LIMIT";
	}

	// 自动清理掉那些原来有、但现在被前端清空的价格记录
	dao.deleteRemovedPrices(instanceId, pKeep, sKeep, lKeep);
}

// ==========================================================
// 列表分页
// ==========================================================
EquipmentDetailPageDTO::Wrapper EquipmentService::listAll(const EquipmentQuery::Wrapper& query)
{
	auto pages = EquipmentDetailPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	EquipmentDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) return pages;

	pages->total = count;
	pages->calcPages();
	list<EquipmentVeiwDO> result = dao.selectWithPage(query);

	for (EquipmentVeiwDO& sub : result)
	{
		auto dto = EquipmentDetailDTO::createShared();

		dto->id = oatpp::String(sub.getId());
		dto->code = oatpp::String(sub.getBusNo());
		dto->name = oatpp::String(sub.getName());
		dto->specification = oatpp::String(sub.getTotalVolume());
		dto->status = oatpp::String(sub.getStatusEnum());
		dto->classification = oatpp::String(sub.getCategoryCode());
		dto->manufacturer = oatpp::String(sub.getManufacturerText());
		dto->price = sub.getSalePrice();

		pages->addData(dto);
	}
	return pages;
}

// ==========================================================
// 通过id查询 (回显完整表单数据)
// ==========================================================
EquipmentDTO::Wrapper EquipmentService::getById(std::string id)
{
	EquipmentDAO dao;
	auto res = dao.selectById(id);
	if (!res) return nullptr;

	auto dto = EquipmentDTO::createShared();
	
	// 系统字段
	dto->id = res->getId();
	dto->status = (res->getStatusEnum() == "ACTIVE") ? 1 : 0; // String 转 Int32
	dto->catalogcode = res->getYbOrgNo();

	// 1. 基础信息
	dto->name = res->getName();
	dto->goods = res->getMerchandiseName();
	dto->classification = res->getCategoryCode();
	dto->manufacturer = res->getManufacturerText();
	dto->supplier = res->getSupplyId();
	dto->apprno = res->getApprovalNumber();
	dto->specification = res->getTotalVolume();
	dto->medcode = res->getYbNo();
	dto->finantype = res->getTypeCode();
	dto->medinscat = res->getYbType();
	dto->medinsgra = res->getChrgitmLv(); // DO 中是 int，DTO 中是 Int32，完美匹配
	dto->is_medins_match = res->getYbMatchFlag();

	// 2. 库存与单位信息 (含三个价格)
	dto->purchaunit = res->getPurchaseUnitCode();
	dto->purmin = res->getPurchaseUnitRatio();
	dto->purchase = res->getPurchasePrice();  // 映射视图中的购入价

	dto->inventory = res->getStockUnitCode();
	dto->invsplit = res->getStockUnitRatio();
	dto->retail = res->getSalePrice();        // 映射视图中的零售价

	dto->packaging = res->getPackageUnitCode();
	dto->packsplit = res->getPackageUnitRatio();
	dto->maxretail = res->getLimitPrice();    // 映射视图中的最高零售价

	dto->split = res->getSplitUnitCode();
	dto->warningmax = res->getItemMaxQuantity();
	dto->warningmin = res->getItemMinQuantity();

	// 3. 业务信息
	dto->pinyin = res->getPyStr();
	dto->wubi = res->getWbStr();

	dto->is_allergic = res->getAllergenFlag();
	dto->is_medins = res->getYbFlag();
	dto->is_high_value = res->getHvcmFlag();
	dto->description = res->getDescription();


	return dto;
}

// ==========================================================
// 新增器材 (主表+价格表)
// ==========================================================
std::string EquipmentService::saveData(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (dao.checkNameExists(dto->name.getValue(""), "")) {
		throw std::runtime_error("Add failed: Equipment name already exists!");
	}
	AdmDeviceDefinitionDO entity;
	UuidFacade uuidFacade;

	std::string newId = uuidFacade.genUuid();
	std::string currentTime = SimpleDateTimeFormat::format();

	entity.setId(newId);
	entity.setTenantId("");
	entity.setCreatedAt(currentTime);
	entity.setUpdatedAt(currentTime);
	entity.setCreatedBy(payload.getId());
	entity.setUpdatedBy(payload.getId());

	// 映射状态 (默认1=ACTIVE)
	int statusVal = dto->status.getValue(1);
	entity.setActiveFlag(statusVal == 1);
	entity.setStatusEnum(statusVal == 1 ? "ACTIVE" : "INACTIVE");

	// 映射基础与业务信息
	entity.setName(dto->name.getValue(""));
	entity.setMerchandiseName(dto->goods.getValue(""));

	//bus_no相关
	std::string dynamicPrefix = "1"; // 兜底默认值
	auto orgMap = payload.getOrganizationUnits();
	if (!orgMap.empty()) {
		dynamicPrefix = orgMap.begin()->first;
	}
	std::string seqKey = dynamicPrefix + ":DD";
	uint64_t nextSeq = dao.generateSequence(seqKey);
	std::ostringstream oss;
	oss << "DD" << std::setw(8) << std::setfill('0') << nextSeq;
	entity.setBusNo(oss.str());

	entity.setCategoryCode(dto->classification.getValue(""));
	entity.setTypeCode(dto->finantype.getValue(""));
	entity.setManufacturerText(dto->manufacturer.getValue(""));
	entity.setSupplyId(dto->supplier.getValue(""));
	entity.setApprovalNumber(dto->apprno.getValue(""));
	entity.setTotalVolume(dto->specification.getValue(""));
	entity.setYbNo(dto->medcode.getValue(""));
	entity.setYbOrgNo(dto->catalogcode.getValue(""));
	entity.setChrgitmLv(dto->medinsgra.getValue(0));
	entity.setPyStr(dto->pinyin.getValue(""));
	entity.setWbStr(dto->wubi.getValue(""));

	entity.setAllergenFlag(dto->is_allergic.getValue(false));
	entity.setYbFlag(dto->is_medins.getValue(false));
	entity.setHvcmFlag(dto->is_high_value.getValue(false));
	entity.setYbMatchFlag(dto->is_medins_match.getValue(false));
	entity.setDescription(dto->description.getValue(""));

	// 映射单位与换算比例 
	entity.setPurchaseUnitCode(dto->purchaunit.getValue(""));
	entity.setPurchaseUnitRatio(dto->purmin.getValue(1.0));
	entity.setStockUnitCode(dto->inventory.getValue(""));
	entity.setStockUnitRatio(dto->invsplit.getValue(1.0));
	entity.setPackageUnitCode(dto->packaging.getValue(""));
	entity.setPackageUnitRatio(dto->packsplit.getValue(1.0));
	entity.setSplitUnitCode(dto->split.getValue(""));

	entity.setItemMinQuantity(dto->warningmin.getValue(0.0));
	entity.setItemMaxQuantity(dto->warningmax.getValue(0.0));

	// 1. 插入主表
	if (dao.insert(entity) > 0) {
		// 2. 主表成功后，插入三条价格到附加表
		_savePrices(dao, newId, entity.getName(), entity.getTypeCode(), dto->medinscat.getValue(""), dto, payload);
		return newId;
	}
	return "";
}

// ==========================================================
// 修改器材 (主表 + 清理旧价格 + 插新价格)
// ==========================================================
bool EquipmentService::modifyData(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (dao.checkNameExists(dto->name.getValue(""), dto->id.getValue(""))) {
		throw std::runtime_error("Modify failed: Equipment name already exists!！");
	}
	AdmDeviceDefinitionDO entity;
	std::string currentTime = SimpleDateTimeFormat::format();

	// 基础映射 (同 saveData)
	entity.setId(dto->id.getValue(""));
	entity.setUpdatedBy(payload.getId());
	entity.setUpdatedAt(currentTime);

	int statusVal = dto->status.getValue(1);
	entity.setActiveFlag(statusVal == 1);
	entity.setStatusEnum(statusVal == 1 ? "ACTIVE" : "INACTIVE");

	entity.setName(dto->name.getValue(""));
	entity.setMerchandiseName(dto->goods.getValue(""));
	entity.setCategoryCode(dto->classification.getValue(""));
	entity.setTypeCode(dto->finantype.getValue(""));
	entity.setManufacturerText(dto->manufacturer.getValue(""));
	entity.setSupplyId(dto->supplier.getValue(""));
	entity.setApprovalNumber(dto->apprno.getValue(""));
	entity.setTotalVolume(dto->specification.getValue(""));

	entity.setYbOrgNo("");
	entity.setYbNo(dto->medinscode.getValue(""));
	int lv = 0;
	if (dto->medinsgra && !dto->medinsgra->empty()) {
		try {
			// 将字符串提取出来，并转换为整型
			lv = std::stoi(dto->medinsgra.getValue("0"));
		}
		catch (...) {
			// 防止前端传了非数字的乱码导致程序崩溃
			lv = 0;
		}
	}
	entity.setChrgitmLv(lv);
	entity.setPyStr(dto->pinyin.getValue(""));
	entity.setWbStr(dto->wubi.getValue(""));

	entity.setAllergenFlag(dto->is_allergic.getValue(false));
	entity.setYbFlag(dto->is_medins.getValue(false));
	entity.setHvcmFlag(dto->is_high_value.getValue(false));
	entity.setYbMatchFlag(dto->is_medins_match.getValue(false));
	entity.setDescription(dto->description.getValue(""));

	// 单位
	entity.setPurchaseUnitCode(dto->purchaunit.getValue(""));
	entity.setStockUnitCode(dto->inventory.getValue(""));
	entity.setPackageUnitCode(dto->packaging.getValue(""));
	entity.setSplitUnitCode(dto->split.getValue(""));

	auto safeStod = [](const std::string& str, double defaultVal = 0.0) -> double {
		try { return str.empty() ? defaultVal : std::stod(str); }
		catch (...) { return defaultVal; }
		};
	entity.setPurchaseUnitRatio(safeStod(dto->purmin.getValue("1.0")));
	entity.setStockUnitRatio(safeStod(dto->invsplit.getValue("1.0")));
	entity.setPackageUnitRatio(safeStod(dto->packsplit.getValue("1.0")));
	entity.setItemMinQuantity(safeStod(dto->warningmin.getValue("0.0")));
	entity.setItemMaxQuantity(safeStod(dto->warningmax.getValue("0.0")));

	std::cout << "========== 准备修改主表 ==========" << std::endl;

	// 执行主表更新 (注意：如果主表信息完全没变，MySQL返回受影响行数为0，所以用 >= 0 判断)
	int updateRows = dao.update(entity);

	if (updateRows >= 0) {
		std::cout << "[主表修改成功] 受影响行数: " << updateRows << std::endl;

		// 构建代理 DTO 传给辅助函数处理价格
		auto priceProxyDto = EquipmentAddDTO::createShared();

		// 安全解析 String 到 Float64
		if (dto->purchase && !dto->purchase->empty()) {
			try { priceProxyDto->purchase = oatpp::Float64(std::stod(dto->purchase->c_str())); }
			catch (...) {}
		}
		if (dto->retail && !dto->retail->empty()) {
			try { priceProxyDto->retail = oatpp::Float64(std::stod(dto->retail->c_str())); }
			catch (...) {}
		}
		if (dto->maxretail && !dto->maxretail->empty()) {
			try { priceProxyDto->maxretail = oatpp::Float64(std::stod(dto->maxretail->c_str())); }
			catch (...) {}
		}

		// 插入新价格 (内部是 UPSERT 逻辑)
		_savePrices(dao, entity.getId(), entity.getName(), entity.getTypeCode(), dto->medinscat.getValue(""), priceProxyDto, payload);

		return true;
	}
	else {
		std::cout << "[主表修改失败] 数据库返回错误！" << std::endl;
	}
	return false;
}

// 批量删除
bool EquipmentService::deleteData(const EquipmentDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (!dto->ids || dto->ids->empty()) return false;

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		idList.push_back(idItem.getValue(""));
	}
	std::string deletedBy = payload.getId();
	std::string deletedAt = SimpleDateTimeFormat::format();
	return dao.softDeleteByIds(idList, deletedBy, deletedAt) > 0;
}

// 批量启停用
bool EquipmentService::changeStatus(const EquipmentStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
	EquipmentDAO dao;
	if (!dto->ids || dto->ids->empty()) return false;

	std::list<std::string> idList;
	for (auto& idItem : *(dto->ids)) {
		idList.push_back(idItem.getValue(""));
	}

	int activeFlag = dto->status.getValue(1);
	// 根据 1 和 0 推导出对应的字符串状态码
	std::string statusEnumStr = (activeFlag == 1) ? "ACTIVE" : "INACTIVE";

	std::string updatedBy = payload.getId();
	std::string updatedAt = SimpleDateTimeFormat::format();

	return dao.updateActiveFlagByIds(idList, activeFlag, statusEnumStr, updatedBy, updatedAt) > 0;
}

// 下拉框精简数据
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

	list<EquipmentVeiwDO> result = dao.selectWithPage(query);
	for (EquipmentVeiwDO& sub : result)
	{
		auto dto = EquipmentSelectDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		dto->code = sub.getBusNo();
		dto->unit = sub.getSplitUnitCode();
		pages->addData(dto);
	}
	return pages;
}