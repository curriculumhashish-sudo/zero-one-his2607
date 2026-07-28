#pragma once
#ifndef _DEPT_DO_
#define _DEPT_DO_

#include "../DoInclude.h"

class DeptDO : public BaseDO
{
    // 科室编号
    MYSQL_SYNTHESIZE(string, id, Id);
    // 租户编号
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    // 创建时间
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    // 修改时间
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    // 创建人
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    // 修改人
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    // 是否删除
    MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
    // 删除时间
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    // 删除人
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    // 版本号
    MYSQL_SYNTHESIZE(int, version, Version);
    // 科室名称
    MYSQL_SYNTHESIZE(string, name, Name);
    // 业务编号
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    // 上级组织编号
    MYSQL_SYNTHESIZE(string, parentId, ParentId);
    // 类型
    MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
    // 类别
    MYSQL_SYNTHESIZE(string, classEnum, ClassEnum);
    // 状态
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    // 显示顺序
    MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
    // 默认医生编号
    MYSQL_SYNTHESIZE(string, defDoctorId, DefDoctorId);
    // 拼音码
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    // 五笔码
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    // 医保编码
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    // 医保名称
    MYSQL_SYNTHESIZE(string, ybName, YbName);
    // 机构标识
    MYSQL_SYNTHESIZE(string, medinsId, MedinsId);
    // 医保区划
    MYSQL_SYNTHESIZE(string, medinsAdmdvs, MedinsAdmdvs);
    // 机构类型
    MYSQL_SYNTHESIZE(string, medinsType, MedinsType);
    // 机构等级
    MYSQL_SYNTHESIZE(string, medinsLv, MedinsLv);
    // 科别
    MYSQL_SYNTHESIZE(string, caty, Caty);

public:
    DeptDO() : BaseDO("adm_organization")
    {
        // 科室编号
        MYSQL_ADD_FIELD_PK("id", "s", id);
        // 租户编号
        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
        // 创建时间
        MYSQL_ADD_FIELD("created_at", "dt", createdAt);
        // 修改时间
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
        // 创建人
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        // 修改人
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        // 是否删除
        MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
        // 删除时间
        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
        // 删除人
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
        // 版本号
        MYSQL_ADD_FIELD("version", "i", version);
        // 科室名称
        MYSQL_ADD_FIELD("name", "s", name);
        // 业务编号
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        // 上级组织编号
        MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId, true);
        // 类型
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        // 类别
        MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", classEnum, true);
        // 状态
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        // 显示顺序
        MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder, true);
        // 默认医生编号
        MYSQL_ADD_FIELD_NULLABLE("def_doctor_id", "s", defDoctorId, true);
        // 拼音码
        MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
        // 五笔码
        MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
        // 医保编码
        MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
        // 医保名称
        MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", ybName, true);
        // 机构标识
        MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medinsId, true);
        // 医保区划
        MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medinsAdmdvs, true);
        // 机构类型
        MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medinsType, true);
        // 机构等级
        MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medinsLv, true);
        // 科别
        MYSQL_ADD_FIELD_NULLABLE("caty", "s", caty, true);
    }
};

typedef std::shared_ptr<DeptDO> PtrDeptDO;

#endif // !_DEPT_DO_
