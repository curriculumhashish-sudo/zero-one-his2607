#pragma once

#ifndef _ADM_DEVICE_DEFINITION_DO_H_
#define _ADM_DEVICE_DEFINITION_DO_H_

#include "../DoInclude.h"

/**
 * 耗材/器材基础定义字典实体 (基于最新 adm_device_definition 结构)
 */
class AdmDeviceDefinitionDO : public BaseDO
{
public:
    // ================== 1. 基础系统字段 ==================
    MYSQL_SYNTHESIZE(std::string, id, Id);                     // 主键
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);         // 租户标识
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);       // 创建时间
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);       // 最后更新时间
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);       // 创建人
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);       // 最后更新人
    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);              // 软删标记
    MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);       // 删除时间
    MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);       // 删除人
    MYSQL_SYNTHESIZE(int, version, Version);                   // 乐观锁版本号

    // ================== 2. 核心业务及分类字段 ==================
    MYSQL_SYNTHESIZE(std::string, name, Name);                 // 器材定义聚合根(名称)
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);               // 器材业务编码
    MYSQL_SYNTHESIZE(std::string, pyStr, PyStr);               // 拼音码
    MYSQL_SYNTHESIZE(std::string, wbStr, WbStr);               // 五笔码
    MYSQL_SYNTHESIZE(std::string, categoryCode, CategoryCode); // 器材分类编码
    MYSQL_SYNTHESIZE(std::string, typeCode, TypeCode);         // 器材种类编码(财务类型)
    MYSQL_SYNTHESIZE(std::string, merchandiseName, MerchandiseName);     // 商品名称
    MYSQL_SYNTHESIZE(std::string, merchandisePyStr, MerchandisePyStr);   // 商品拼音码
    MYSQL_SYNTHESIZE(std::string, merchandiseWbStr, MerchandiseWbStr);   // 商品五笔码

    // ================== 3. 单位与规格字段 ==================
    MYSQL_SYNTHESIZE(std::string, purchaseUnitCode, PurchaseUnitCode); // 采购单位
    MYSQL_SYNTHESIZE(std::string, splitUnitCode, SplitUnitCode);       // 拆零单位
    MYSQL_SYNTHESIZE(std::string, doseUnitCode, DoseUnitCode);         // 剂量单位 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, packageUnitCode, PackageUnitCode);   // 销售单位/包装单位 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, stockUnitCode, StockUnitCode);       // 库存单位 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, totalVolume, TotalVolume);           // 包装规格描述
    MYSQL_SYNTHESIZE(double, itemMinQuantity, ItemMinQuantity);        // 最小库存警戒量
    MYSQL_SYNTHESIZE(double, itemMaxQuantity, ItemMaxQuantity);        // 最大库存警戒量

    // ================== 4. 换算比例字段 ==================
    MYSQL_SYNTHESIZE(double, doseUnitRatio, DoseUnitRatio);         // 剂量单位→拆零换算
    MYSQL_SYNTHESIZE(double, purchaseUnitRatio, PurchaseUnitRatio); // 采购单位→拆零换算
    MYSQL_SYNTHESIZE(double, stockUnitRatio, StockUnitRatio);       // 库存单位→拆零换算
    MYSQL_SYNTHESIZE(double, packageUnitRatio, PackageUnitRatio);   // 销售单位→拆零换算

    // ================== 5. 厂家与供应商字段 ==================
    MYSQL_SYNTHESIZE(std::string, approvalNumber, ApprovalNumber);     // 批准文号
    MYSQL_SYNTHESIZE(std::string, manufacturerId, ManufacturerId);     // 生产厂家标识
    MYSQL_SYNTHESIZE(std::string, manufacturerText, ManufacturerText); // 生产厂家名称
    MYSQL_SYNTHESIZE(std::string, supplyId, SupplyId);                 // 供应商标识
    MYSQL_SYNTHESIZE(std::string, modelNumber, ModelNumber);           // 产品型号

    // ================== 6. 医疗与状态标记字段 ==================
    MYSQL_SYNTHESIZE(std::string, defEncounterUnitCode, DefEncounterUnitCode);   // 默认门诊单位 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, defInhospitalUnitCode, DefInhospitalUnitCode); // 默认住院单位 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, orgId, OrgId);               // 所属科室标识
    MYSQL_SYNTHESIZE(std::string, locationId, LocationId);     // 所在位置标识 (新增补齐)
    MYSQL_SYNTHESIZE(bool, hvcmFlag, HvcmFlag);                // 高值器材标志
    MYSQL_SYNTHESIZE(bool, allergenFlag, AllergenFlag);        // 过敏标记
    MYSQL_SYNTHESIZE(bool, rxFlag, RxFlag);                    // 处方标志
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);     // 发布状态
    MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);            // 是否启用标记

    // ================== 7. 文本描述字段 (前端说明) ==================
    MYSQL_SYNTHESIZE(std::string, description, Description);       // 器材说明 (新增补齐，对应前端的“说明”)
    MYSQL_SYNTHESIZE(std::string, jurisdiction, Jurisdiction);     // 适用范围描述 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, deviceVersion, DeviceVersion);   // 器材版本 (新增补齐)
    MYSQL_SYNTHESIZE(std::string, substanceText, SubstanceText);   // 主要成分描述 (新增补齐)

    // ================== 8. 医保相关字段 ==================
    MYSQL_SYNTHESIZE(bool, ybFlag, YbFlag);                        // 医保标记
    MYSQL_SYNTHESIZE(std::string, ybNo, YbNo);                     // 医保编码
    MYSQL_SYNTHESIZE(std::string, ybOrgNo, YbOrgNo);               // 医药机构目录编码 (新增补齐)
    MYSQL_SYNTHESIZE(bool, ybMatchFlag, YbMatchFlag);              // 医保对码标记
    MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);                   // 医保等级
    MYSQL_SYNTHESIZE(std::string, ybMappingCode, YbMappingCode);   // 医保对码标识 (新增补齐)

public:
    AdmDeviceDefinitionDO() : BaseDO("adm_device_definition")
    {
        // ================== 映射主键 ==================
        MYSQL_ADD_FIELD_PK("id", "s", id);

        // ================== 映射系统字段 ==================
        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, false);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

        // ================== 映射业务字段 ==================
        MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
        MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
        MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
        MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
        MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, true);
        MYSQL_ADD_FIELD_NULLABLE("type_code", "s", typeCode, true);
        MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
        MYSQL_ADD_FIELD_NULLABLE("merchandise_py_str", "s", merchandisePyStr, true);
        MYSQL_ADD_FIELD_NULLABLE("merchandise_wb_str", "s", merchandiseWbStr, true);

        // ================== 映射单位与规格 ==================
        MYSQL_ADD_FIELD_NULLABLE("purchase_unit_code", "s", purchaseUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("split_unit_code", "s", splitUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("package_unit_code", "s", packageUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("stock_unit_code", "s", stockUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("total_volume", "s", totalVolume, true);
        MYSQL_ADD_FIELD_NULLABLE("item_min_quantity", "d", itemMinQuantity, true);
        MYSQL_ADD_FIELD_NULLABLE("item_max_quantity", "d", itemMaxQuantity, true);

        // ================== 映射换算比例 ==================
        MYSQL_ADD_FIELD_NULLABLE("dose_unit_ratio", "d", doseUnitRatio, true);
        MYSQL_ADD_FIELD_NULLABLE("purchase_unit_ratio", "d", purchaseUnitRatio, true);
        MYSQL_ADD_FIELD_NULLABLE("stock_unit_ratio", "d", stockUnitRatio, true);
        MYSQL_ADD_FIELD_NULLABLE("package_unit_ratio", "d", packageUnitRatio, true);

        // ================== 映射厂家供应商等 ==================
        MYSQL_ADD_FIELD_NULLABLE("approval_number", "s", approvalNumber, true);
        MYSQL_ADD_FIELD_NULLABLE("manufacturer_id", "s", manufacturerId, true);
        MYSQL_ADD_FIELD_NULLABLE("manufacturer_text", "s", manufacturerText, true);
        MYSQL_ADD_FIELD_NULLABLE("supply_id", "s", supplyId, true);
        MYSQL_ADD_FIELD_NULLABLE("model_number", "s", modelNumber, true);

        // ================== 映射医疗与状态标记 ==================
        MYSQL_ADD_FIELD_NULLABLE("def_encounter_unit_code", "s", defEncounterUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("def_inhospital_unit_code", "s", defInhospitalUnitCode, true);
        MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
        MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
        MYSQL_ADD_FIELD_NULLABLE("hvcm_flag", "b", hvcmFlag, true);
        MYSQL_ADD_FIELD_NULLABLE("allergen_flag", "b", allergenFlag, true);
        MYSQL_ADD_FIELD_NULLABLE("rx_flag", "b", rxFlag, true);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD_NULLABLE("active_flag", "b", activeFlag, false);

        // ================== 映射文本描述 ==================
        MYSQL_ADD_FIELD_NULLABLE("description", "s", description, true);
        MYSQL_ADD_FIELD_NULLABLE("jurisdiction", "s", jurisdiction, true);
        MYSQL_ADD_FIELD_NULLABLE("device_version", "s", deviceVersion, true);
        MYSQL_ADD_FIELD_NULLABLE("substance_text", "s", substanceText, true);

        // ================== 映射医保相关 ==================
        MYSQL_ADD_FIELD_NULLABLE("yb_flag", "b", ybFlag, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_org_no", "s", ybOrgNo, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "b", ybMatchFlag, true);
        MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);
        MYSQL_ADD_FIELD_NULLABLE("yb_mapping_code", "s", ybMappingCode, true);
    }
};


// 智能指针
typedef std::shared_ptr<AdmDeviceDefinitionDO> PtrAdmDeviceDefinitionDO;

class EquipmentVeiwDO : public AdmDeviceDefinitionDO {

    MYSQL_SYNTHESIZE(double, purchasePrice, PurchasePrice); // 购入价
    MYSQL_SYNTHESIZE(double, salePrice, SalePrice);         // 零售价
    MYSQL_SYNTHESIZE(double, limitPrice, LimitPrice);       // 最高零售价
    MYSQL_SYNTHESIZE(std::string, ybType, YbType);      // 医保类别
};

typedef std::shared_ptr<EquipmentVeiwDO> PtrEquipmentVeiwDO;
#endif // !_ADM_DEVICE_DEFINITION_DO_H_