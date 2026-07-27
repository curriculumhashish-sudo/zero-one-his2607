#pragma once

#ifndef _DEPT_DO_
#define _DEPT_DO_
#include "../DoInclude.h"

/**
 * 科室机构表 adm_organization 数据库实体DO
 * 完整映射表所有字段，遵循项目MYSQL_SYNTHESIZE宏规范
 */
class DeptDO : public BaseDO
{
	// 主键 id bigint
	MYSQL_SYNTHESIZE(int64_t, id, Id);
	// 租户ID
	MYSQL_SYNTHESIZE(int64_t, tenant_id, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, created_at, CreatedAt);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updated_at, UpdatedAt);
	// 创建人ID
	MYSQL_SYNTHESIZE(int64_t, created_by, CreatedBy);
	// 最后更新人ID
	MYSQL_SYNTHESIZE(int64_t, updated_by, UpdatedBy);
	// 软删除标记 bool
	MYSQL_SYNTHESIZE(bool, is_deleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deleted_at, DeletedAt);
	// 删除人ID
	MYSQL_SYNTHESIZE(int64_t, deleted_by, DeletedBy);
	// 乐观锁版本号 int
	MYSQL_SYNTHESIZE(int32_t, version, Version);
	// 科室名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// bus编号
	MYSQL_SYNTHESIZE(string, bus_no, BusNo);
	// 父级科室ID
	MYSQL_SYNTHESIZE(int64_t, parent_id, ParentId);
	// type枚举
	MYSQL_SYNTHESIZE(string, type_enum, TypeEnum);
	// class分类枚举
	MYSQL_SYNTHESIZE(string, class_enum, ClassEnum);
	// 启停状态枚举 ACTIVE/INACTIVE
	MYSQL_SYNTHESIZE(string, status_enum, StatusEnum);
	// 启停布尔标记 true/false
	MYSQL_SYNTHESIZE(bool, active_flag, ActiveFlag);
	// 显示排序序号
	MYSQL_SYNTHESIZE(int32_t, display_order, DisplayOrder);
	// 默认医生ID
	MYSQL_SYNTHESIZE(int64_t, def_doctor_id, DefDoctorId);
	// 拼音码
	MYSQL_SYNTHESIZE(string, py_str, PyStr);
	// 五笔码
	MYSQL_SYNTHESIZE(string, wb_str, WbStr);
	// 医保编号
	MYSQL_SYNTHESIZE(string, yb_no, YbNo);
	// 医保名称
	MYSQL_SYNTHESIZE(string, yb_name, YbName);
	// 医疗机构ID
	MYSQL_SYNTHESIZE(string, medins_id, MedinsId);
	// 医疗机构医保区划
	MYSQL_SYNTHESIZE(string, medins_admdvs, MedinsAdmdvs);
	// 医疗机构类型
	MYSQL_SYNTHESIZE(string, medins_type, MedinsType);
	// 医疗机构等级
	MYSQL_SYNTHESIZE(string, medins_lv, MedinsLv);
	// 科别
	MYSQL_SYNTHESIZE(string, caty, Caty);

public:
	// 构造函数：传入表名 adm_organization，依次绑定所有字段
	DeptDO() : BaseDO("adm_organization")
	{
		// 主键字段 id  bigint 类型占位符 ll
		MYSQL_ADD_FIELD_PK("id", "ll", id);

		// 普通字段依次绑定（类型占位符对照SqlSession宏规则）
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "ll", tenant_id, true);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updated_at, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "ll", created_by, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "ll", updated_by, true);
		MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deleted_at, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "ll", deleted_by, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "ll", parent_id, true);
		MYSQL_ADD_FIELD("type_enum", "s", type_enum);
		MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", class_enum, true);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("active_flag", "b", active_flag);
		MYSQL_ADD_FIELD_NULLABLE("display_order", "i", display_order, true);
		MYSQL_ADD_FIELD_NULLABLE("def_doctor_id", "ll", def_doctor_id, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", py_str, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wb_str, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", yb_no, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", yb_name, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medins_id, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medins_admdvs, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medins_type, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medins_lv, true);
		MYSQL_ADD_FIELD_NULLABLE("caty", "s", caty, true);
	}
};

#endif // !_DEPT_DO_
