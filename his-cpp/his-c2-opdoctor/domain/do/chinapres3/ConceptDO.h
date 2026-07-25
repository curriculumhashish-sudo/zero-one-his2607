#pragma once
#pragma once

#ifndef _CONCEPTDO_H_
#define _CONCEPTDO_H_

#include "../DoInclude.h"

/**
 * 编码概念DO
 *
 * 对应数据库：
 * ter_concept
 *
 * 业务作用：
 *
 * 1. 提供系统公共编码查询
 * 2. 用于处方签发页面下拉选择
 *
 * 当前使用场景：
 *
 * 用法：
 *   煎服
 *   温服
 *
 * 频次、其他枚举类数据
 * 也可能通过该表维护
 *
 * 注意：
 *
 * 1. 本DO只用于查询
 * 2. 不负责新增和修改
 * 3. code用于业务保存
 * 4. display用于页面展示
 */
class ConceptDO : public BaseDO
{
	/**
	 * 数据库主键
	 *
	 * 字段：
	 * id
	 *
	 * 表示一条编码记录
	 */
	MYSQL_SYNTHESIZE(string, id, Id);

	/**
	 * 编码体系ID
	 *
	 * 字段：
	 * code_system_id
	 *
	 * 用于区分不同类型编码
	 *
	 */
	MYSQL_SYNTHESIZE(string, codeSystemId, CodeSystemId);

	/**
	 * 编码值
	 *
	 * 字段：
	 * code
	 *
	 * 实际保存到业务表中的值

	 */
	MYSQL_SYNTHESIZE(string, code, Code);

	/**
	 * 显示名称
	 *
	 * 字段：
	 * display
	 *
	 * 前端展示内容
	 *
	 * 示例：
	 * 温服
	 */
	MYSQL_SYNTHESIZE(string, display, Display);

	/**
	 * 排序号
	 *
	 * 字段：
	 * sort_order
	 *
	 * 用于下拉列表排序
	 */
	MYSQL_SYNTHESIZE(long long, sortOrder, SortOrder);

	/**
	 * 是否启用
	 *
	 * 字段：
	 * is_active
	 *
	 * 过滤无效数据
	 */
	MYSQL_SYNTHESIZE(bool, isActive, IsActive);

public:
	/**
	 * 构造函数
	 *
	 * 对应数据表：
	 * ter_concept
	 */
	ConceptDO()
		: BaseDO("ter_concept")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);
		// 编码体系ID
		MYSQL_ADD_FIELD("code_system_id", "s", codeSystemId);
		// 编码
		MYSQL_ADD_FIELD("code", "s", code);
		// 显示名称
		MYSQL_ADD_FIELD("display", "s", display);
		// 排序
		MYSQL_ADD_FIELD_NULLABLE("sort_order", "ll", sortOrder, true);
		// 是否启用
		MYSQL_ADD_FIELD("is_active", "b", isActive);
	}
};

// 智能指针别名
typedef std::shared_ptr<ConceptDO> PtrConceptDO;

#endif // !_CONCEPTDO_H_