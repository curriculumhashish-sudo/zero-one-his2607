#pragma once

#ifndef _ADM_CHARGE_ITEM_DEFINITION_DO_H_
#define _ADM_CHARGE_ITEM_DEFINITION_DO_H_

#include "../DoInclude.h"
#include <string>
#include <memory>

/**
 * 收费项目定义实体
 * 描述某业务实例在特定价类/批次下的可计费项与价格
 */
class AdmChargeItemDefinitionDO : public BaseDO
{
public:
    // ================== 1. 基础系统字段 ==================
    MYSQL_SYNTHESIZE(std::string, id, Id);                     // 主键
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);         // 租户标识
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);       // 创建时间
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);       // 最后更新时间
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);       // 创建人
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);       // 最后更新人
    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);              // 软删标记 (0-正常 1-已删除)
    MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);       // 删除时间
    MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);       // 删除人
    MYSQL_SYNTHESIZE(int, version, Version);                   // 乐观锁版本号

    // ================== 2. 核心关联字段 ==================
    MYSQL_SYNTHESIZE(std::string, instanceTable, InstanceTable); // 收费项目定义实体 (此处应填 "adm_device_definition")
    MYSQL_SYNTHESIZE(std::string, instanceId, InstanceId);       // 业务实例主键 (关联耗材的 ID)
    MYSQL_SYNTHESIZE(std::string, orgId, OrgId);                 // 所属机构主键（可空表示全局）

    // ================== 3. 业务收费字段 ==================
    MYSQL_SYNTHESIZE(std::string, chargeName, ChargeName);       // 收费展示名称
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);       // 状态枚举编码（默认 ACTIVE）
    MYSQL_SYNTHESIZE(std::string, typeCode, TypeCode);           // 收费类型编码（必填）
    MYSQL_SYNTHESIZE(std::string, ybType, YbType);               // 医保类别或对照编码 (映射前端的医保类别)
    MYSQL_SYNTHESIZE(std::string, priceTypeCode, PriceTypeCode); // 价格类型编码 (必填，如购入价、零售价的字典码)
    MYSQL_SYNTHESIZE(std::string, lotNo, LotNo);                 // 批次或效期批次号
    MYSQL_SYNTHESIZE(double, priceValue, PriceValue);            // 价格数值（元）
    MYSQL_SYNTHESIZE(int, priority, Priority);                   // 同实例多价时的优先级

public:
    // 构造函数：绑定数据库表名 adm_charge_item_definition
    AdmChargeItemDefinitionDO() : BaseDO("adm_charge_item_definition")
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

        // ================== 映射关联与业务字段 ==================
        MYSQL_ADD_FIELD_NULLABLE("instance_table", "s", instanceTable, true);
        MYSQL_ADD_FIELD_NULLABLE("instance_id", "s", instanceId, false); // 必须有关联的实例 ID
        MYSQL_ADD_FIELD_NULLABLE("charge_name", "s", chargeName, true);
        MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
        MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
        MYSQL_ADD_FIELD_NULLABLE("type_code", "s", typeCode, false);
        MYSQL_ADD_FIELD_NULLABLE("yb_type", "s", ybType, true);
        MYSQL_ADD_FIELD_NULLABLE("price_type_code", "s", priceTypeCode, false);
        MYSQL_ADD_FIELD_NULLABLE("lot_no", "s", lotNo, true);
        MYSQL_ADD_FIELD_NULLABLE("price_value", "d", priceValue, false);
        MYSQL_ADD_FIELD_NULLABLE("priority", "i", priority, true);
    }
};

// 智能指针类型定义
typedef std::shared_ptr<AdmChargeItemDefinitionDO> PtrAdmChargeItemDefinitionDO;

#endif // !_ADM_CHARGE_ITEM_DEFINITION_DO_H_