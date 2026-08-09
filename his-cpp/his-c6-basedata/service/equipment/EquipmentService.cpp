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
	list<EquipmentVeiwDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (EquipmentVeiwDO& sub : result)
	{
		auto dto = EquipmentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, 
			// id
			id, Id,
			// 编码
			code, BusNo,
			// 项目名
			name, Name,
			// 规格
			specification, TotalVolume,
			// 状态
			status, StatusEnum,
			// 分类
			classification, CategoryCode,
			// 厂家
			manufacturer, ManufacturerText,
			// 价格
			price, SalePrice
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
		medcode, YbNo,
		// 财务类型
		finantype, TypeCode,
		// 目录编码
		catalogcode, YbOrgNo,
		// 医保类别
		medinscat, YbType,
		// 医保等级
		medinsgra, ChrgitmLv,
		// 采购单位编码
		purchaunit, PurchaseUnitCode,
		// 采购/拆零倍数（采购换算比例）
		purmin, PurchaseUnitRatio,
		// 购入价（整包装价格）
		purchase, PurchasePrice,
		// 库存单位编码
		inventory, SplitUnitCode,
		// 库存拆零倍数（库存换算比例）
		invsplit, StockUnitRatio,
		// 零售价（整包装售价）
		retail, SalePrice,
		// 包装单位编码
		packaging, PackageUnitCode,
		// 包装拆零倍数（包装换算比例）
		packsplit, PackageUnitRatio,
		// 最高零售价
		maxretail, LimitPrice,
		// 拆零单位编码
		split, SplitUnitCode,
		// 库存警戒上限
		warningmax, ItemMaxQuantity,
		// 库存警戒下限
		warningmin, ItemMinQuantity,
		// 拼音检索码
		pinyin, PyStr,
		// 五笔检索码
		wubi, WbStr
	);
	ZO_STAR_DOMAIN_DO_TO_DTO_1(
		dto, res,
		// 医保对码标记
		ybmatchflag, YbMatchFlag,
		// 过敏标记
		allergenflag, AllergenFlag,
		// 医保标记
		ybflag, YbFlag,
		// 高值器材标志
		hvcmflag, HvcmFlag
	)
	return dto;
}

// 新增器材
std::string EquipmentService::saveData(const EquipmentAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	//检测JSON是否为空，安全凭证是否正确被解析
	if (!dto) {
		throw std::runtime_error("请求体解析失败，传入的 JSON 为空或格式不正确！");
	}
	if (&payload == nullptr) {
		throw std::runtime_error("安全凭证(Token)解析失败，操作人信息为空！");
	}

	EquipmentDAO dao;
	AdmDeviceDefinitionDO entity;

	UuidFacade uuidFacade;
	std::string newId = uuidFacade.genUuid();
	entity.setId(newId);
	
	std::string currentTime = SimpleDateTimeFormat::format();
	entity.setTenantId("");       // 强制赋空串，解决 getTenantId 崩溃
	entity.setCreatedAt(currentTime);
	entity.setUpdatedAt(currentTime); 
	entity.setCreatedBy(payload.getId());
	entity.setUpdatedBy(payload.getId());

	entity.setAllergenFlag(false);
	entity.setYbFlag(false);
	entity.setHvcmFlag(false);
	entity.setYbMatchFlag(false);
	entity.setDescription("");


	entity.setCreatedBy(payload.getId());
	entity.setCreatedAt(SimpleDateTimeFormat::format());

	// 2. 状态映射 (DTO 里的 status 是 Int32: 1或0)
	int statusVal = dto->status.getValue(1);
	entity.setActiveFlag(statusVal == 1);
	entity.setStatusEnum(statusVal == 1 ? "ACTIVE" : "INACTIVE");

	// 3. 基础与业务信息映射 (使用 dto->xxx.getValue("") 防空指针)
	entity.setName(dto->name.getValue(""));
	entity.setMerchandiseName(dto->goods.getValue(""));
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

	// 4. 单位与换算比例 (DTO 里定义的是 Float64)
	entity.setPurchaseUnitCode(dto->purchaunit.getValue(""));
	entity.setPurchaseUnitRatio(dto->purmin.getValue(1.0));
	entity.setStockUnitCode(dto->inventory.getValue(""));
	entity.setStockUnitRatio(dto->invsplit.getValue(1.0));
	entity.setPackageUnitCode(dto->packaging.getValue(""));
	entity.setPackageUnitRatio(dto->packsplit.getValue(1.0));
	entity.setSplitUnitCode(dto->split.getValue(""));

	entity.setItemMinQuantity(dto->warningmin.getValue(0.0));
	entity.setItemMaxQuantity(dto->warningmax.getValue(0.0));

	if (dao.insert(entity) > 0) {
		return newId;
	}
	return "";
}

// 修改器材
bool EquipmentService::modifyData(const EquipmentModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	//检测JSON是否为空，安全凭证是否正确被解析
	if (!dto) {
		throw std::runtime_error("请求体解析失败，传入的 JSON 为空或格式不正确！");
	}
	if (&payload == nullptr) {
		throw std::runtime_error("安全凭证(Token)解析失败，操作人信息为空！");
	}

	EquipmentDAO dao;
	AdmDeviceDefinitionDO  entity;

	std::string currentTime = SimpleDateTimeFormat::format();
	entity.setTenantId("");       // 强制赋空串，解决 getTenantId 崩溃
	entity.setUpdatedAt(currentTime);
	entity.setCreatedAt(currentTime);      // 新增时没有更新时间
	entity.setUpdatedBy(payload.getId());
	entity.setCreatedBy("");      // 新增时没有更新人

	entity.setYbOrgNo("");
	entity.setDescription("");
	entity.setAllergenFlag(false);
	entity.setYbFlag(false);
	entity.setHvcmFlag(false);
	entity.setYbMatchFlag(false);


	

	// 1. 系统字段
	entity.setId(dto->id.getValue(""));
	entity.setUpdatedBy(payload.getId());
	entity.setUpdatedAt(SimpleDateTimeFormat::format());

	// 2. 状态映射
	int statusVal = dto->status.getValue(1);
	entity.setActiveFlag(statusVal == 1);
	entity.setStatusEnum(statusVal == 1 ? "ACTIVE" : "INACTIVE");

	// 3. 基础与业务信息映射
	entity.setName(dto->name.getValue(""));
	entity.setMerchandiseName(dto->goods.getValue(""));
	entity.setCategoryCode(dto->classification.getValue(""));
	entity.setTypeCode(dto->finantype.getValue(""));
	entity.setManufacturerText(dto->manufacturer.getValue(""));
	entity.setSupplyId(dto->supplier.getValue(""));
	entity.setApprovalNumber(dto->apprno.getValue(""));
	entity.setTotalVolume(dto->specification.getValue(""));
	entity.setYbNo(dto->code.getValue("")); // ModifyDTO 里的医保编码叫 code
	entity.setPyStr(dto->pinyin.getValue(""));
	entity.setWbStr(dto->wubi.getValue(""));

	// 4. 单位映射
	entity.setPurchaseUnitCode(dto->purchaunit.getValue(""));
	entity.setStockUnitCode(dto->inventory.getValue(""));
	entity.setPackageUnitCode(dto->packaging.getValue(""));
	entity.setSplitUnitCode(dto->split.getValue(""));

	int lv = 0;
	if (dto->medinsgra && !dto->medinsgra->empty()) {
		try { lv = std::stoi(dto->medinsgra.getValue("0")); }
		catch (...) {}
	}
	entity.setChrgitmLv(lv);

	auto safeStod = [](const std::string& str, double defaultVal = 0.0) -> double {
		try { return str.empty() ? defaultVal : std::stod(str); }
		catch (...) { return defaultVal; }
		};

	entity.setPurchaseUnitRatio(safeStod(dto->purmin.getValue("1.0")));
	entity.setStockUnitRatio(safeStod(dto->invsplit.getValue("1.0")));
	entity.setPackageUnitRatio(safeStod(dto->packsplit.getValue("1.0")));
	entity.setItemMinQuantity(safeStod(dto->warningmin.getValue("0.0")));
	entity.setItemMaxQuantity(safeStod(dto->warningmax.getValue("0.0")));

	// 更新主表
	// TODO: 价格的修改同样需要单独调用价格表的 Update 方法
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

	// 修正：直接获取 1(启用) 或 0(停用)
	int activeFlag = dto->status.getValue(1);
	std::string updatedBy = payload.getId();
	std::string updatedAt = SimpleDateTimeFormat::format();

	// 修正：调用 DAO 里专门修改 active_flag 的方法
	return dao.updateActiveFlagByIds(idList, activeFlag, updatedBy, updatedAt) > 0;
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

	list<EquipmentVeiwDO> result = dao.selectWithPage(query);
	for (EquipmentVeiwDO& sub : result)
	{
		auto dto = EquipmentSelectDTO::createShared();
		dto->id = sub.getId();
		dto->name = sub.getName();
		dto->code = sub.getBusNo(); // 补充编码映射
		dto->specification = sub.getTotalVolume(); // 补充规格映射
		dto->manufacturer = sub.getManufacturerText(); // 补充厂家映射
		pages->addData(dto);
	}
	return pages;
}