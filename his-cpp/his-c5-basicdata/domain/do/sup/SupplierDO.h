#pragma once
#ifndef _SUPPLIER_DO_H_
#define _SUPPLIER_DO_H_

#include "../DoInclude.h"

/**
 * 供应商数据实体类
 *
 * 映射 cus_supplier 自定义供应商表的全部字段，
 * 包含审计字段、软删字段和乐观锁版本号。
 */
class SupplierDO : public BaseDO
{
    // 主键
    MYSQL_SYNTHESIZE(string, id, Id);

    // 租户标识
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);

    // 创建时间
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    // 最后更新时间
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    // 创建人
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    // 最后更新人
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);

    // 软删标记
    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
    // 删除时间
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    // 删除人
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

    // 乐观锁版本号
    MYSQL_SYNTHESIZE(int, version, Version);

    // 业务编号（如 SUPP001）
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    // 供应商名称
    MYSQL_SYNTHESIZE(string, name, Name);
    // 类型枚举值（1=生产厂家，2=供应商）
    MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
    // 统一社会信用代码
    MYSQL_SYNTHESIZE(string, creditCode, CreditCode);
    // 法定代表人
    MYSQL_SYNTHESIZE(string, legalRepresentative, LegalRepresentative);
    // 地址
    MYSQL_SYNTHESIZE(string, address, Address);
    // 拼音码
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    // 五笔码
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    // 联系电话
    MYSQL_SYNTHESIZE(string, phone, Phone);
    // 邮箱
    MYSQL_SYNTHESIZE(string, email, Email);
    // 开户行名称
    MYSQL_SYNTHESIZE(string, bankName, BankName);
    // 银行账号
    MYSQL_SYNTHESIZE(string, bankAccount, BankAccount);
    // 机构标识
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
    // 显示顺序
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
    // 状态枚举值（ACTIVE / DISABLED）
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    // 备注说明
    MYSQL_SYNTHESIZE(string, note, Note);

public:
    SupplierDO() : BaseDO("cus_supplier")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);

        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);

        MYSQL_ADD_FIELD("created_at", "dt", createdAt);
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);

        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);

        MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);

        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);

        MYSQL_ADD_FIELD("version", "i", version);

        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        MYSQL_ADD_FIELD("credit_code", "s", creditCode);
        MYSQL_ADD_FIELD_NULLABLE("legal_representative", "s", legalRepresentative, true);
        MYSQL_ADD_FIELD_NULLABLE("address", "s", address, true);
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        MYSQL_ADD_FIELD_NULLABLE("phone", "s", phone, true);
        MYSQL_ADD_FIELD_NULLABLE("email", "s", email, true);
        MYSQL_ADD_FIELD_NULLABLE("bank_name", "s", bankName, true);
        MYSQL_ADD_FIELD_NULLABLE("bank_account", "s", bankAccount, true);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
        MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder, true);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD_NULLABLE("note", "s", note, true);
    }
};

typedef std::shared_ptr<SupplierDO> PtrSupplierDO;

#endif // !_SUPPLIER_DO_H_
