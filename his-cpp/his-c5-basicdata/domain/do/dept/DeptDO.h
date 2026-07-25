#pragma once

#ifndef _DEPT_DO_
#define _DEPT_DO_
#include "../DoInclude.h"

class DeptDO : public BaseDO
{
    // 科室编号
    MYSQL_SYNTHESIZE(string, id, Id);
    // 科室名称
    MYSQL_SYNTHESIZE(string, name, Name);
    // 类型
    MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
    // 类别
    MYSQL_SYNTHESIZE(string, classEnum, ClassEnum);
    // 上级组织编号
    MYSQL_SYNTHESIZE(string, parentId, ParentId);
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
    // 拼音码
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    // 五笔码
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    // 默认医生编号
    MYSQL_SYNTHESIZE(string, defDoctorId, DefDoctorId);
    // 科别
    MYSQL_SYNTHESIZE(string, caty, Caty);
    // 显示顺序
    MYSQL_SYNTHESIZE(string, displayOrder, DisplayOrder);
    // 状态
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    // 是否启用
    MYSQL_SYNTHESIZE(string, activeFlag, ActiveFlag);

    // 创建人
    MYSQL_SYNTHESIZE(string, createBy, CreateBy);
    // 创建时间
    MYSQL_SYNTHESIZE(string, createTime, CreateTime);
    // 修改人
    MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
    // 修改时间
    MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);
public:
    DeptDO() : BaseDO("table_name")
    {
        // 科室编号
        MYSQL_ADD_FIELD_PK("id", "s", id);
        // 科室名称
        MYSQL_ADD_FIELD("name", "s", name);
        // 类型
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        // 类别
        MYSQL_ADD_FIELD("class_enum", "s", classEnum);
        // 上级组织编号
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        // 医保编码
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        // 医保名称
        MYSQL_ADD_FIELD("yb_name", "s", ybName);
        // 机构标识
        MYSQL_ADD_FIELD("medins_id", "s", medinsId);
        // 医保区划
        MYSQL_ADD_FIELD("medins_admdvs", "s", medinsAdmdvs);
        // 机构类型
        MYSQL_ADD_FIELD("medins_type", "s", medinsType);
        // 机构等级
        MYSQL_ADD_FIELD("medins_lv", "s", medinsLv);
        // 拼音码
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        // 五笔码
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        // 默认医生编号
        MYSQL_ADD_FIELD("def_doctor_id", "s", defDoctorId);
        // 科别
        MYSQL_ADD_FIELD("caty", "s", caty);
        // 显示顺序
        MYSQL_ADD_FIELD("display_order", "s", displayOrder);
        // 状态
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        // 是否启用
        MYSQL_ADD_FIELD("active_flag", "s", activeFlag);

        // 创建人
        MYSQL_ADD_FIELD("create_by", "s", createBy);
        // 创建时间
        MYSQL_ADD_FIELD("create_time", "s", createTime);
        // 修改人
        MYSQL_ADD_FIELD("update_by", "s", updateBy);
        // 修改时间
        MYSQL_ADD_FIELD("update_time", "s", updateTime);
    }
};

typedef std::shared_ptr<DeptDO> PtrDeptDO;
#endif // !_DEPT_DO_
