#pragma once

#ifndef _chinapres3ConditionDO_H_
#define _chinapres3ConditionDO_H_

#include "../DoInclude.h"

/**
 * 临床诊断DO
 *
 * 对应数据库：
 * cln_condition
 *
 * 业务作用：
 * 1. 查询患者已有诊断信息
 * 2. 在中医处方签发时提供诊断选择
 * 3. 将选择的诊断关联到处方
 *
 * 注意：
 * 1. 本DO只用于查询
 * 2. 不负责诊断新增和修改
 * 3. id为数据库记录ID，不是处方编号
 */
class chinapres3ConditionDO : public BaseDO
{
	/**
	 * 数据库主键
	 *
	 * 对应字段：
	 * id
	 *
	 * 表示一条诊断记录
	 */
	MYSQL_SYNTHESIZE(string, id, Id);

	/**
	 * 患者ID
	 *
	 * 对应字段：
	 * patient_id
	 *
	 * 用于查询某个患者已有诊断
	 */
	MYSQL_SYNTHESIZE(string, patientId, PatientId);

	/**
	 * 诊断名称/描述
	 *
	 * 对应字段：
	 * description
	 *
	 * 页面显示内容
	 *
	 * 示例：
	 * 气血两虚
	 * 脾胃虚弱
	 */
	MYSQL_SYNTHESIZE(string, description, Description);

	/**
	 * 诊断定义ID
	 *
	 * 对应字段：
	 * definition_id
	 *
	 * 关联：
	 * cln_condition_definition
	 *
	 * 用于标识诊断定义来源
	 */
	MYSQL_SYNTHESIZE(string, definitionId, DefinitionId);

	/**
	 * 是否中医诊断标识
	 *
	 * 对应字段：
	 * tcm_flag
	 *
	 * true表示中医诊断
	 */
	MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);

	/**
	 * 就诊ID
	 *
	 * 对应字段：
	 * encounter_id
	 *
	 * 对应一次门诊就诊记录
	 */
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);

public:
	/**
	 * 构造函数
	 *
	 * 绑定数据库表：
	 * cln_condition
	 */
	chinapres3ConditionDO() : BaseDO("cln_condition")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);
		// 患者ID
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		// 诊断描述
		MYSQL_ADD_FIELD_NULLABLE("description", "s", description, true);
		// 诊断定义ID
		MYSQL_ADD_FIELD_NULLABLE("definition_id", "s", definitionId, true);
		// 是否中医诊断
		MYSQL_ADD_FIELD_NULLABLE("tcm_flag", "b", tcmFlag, true);
		// 就诊ID
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, true);
	}
};

// 智能指针别名
typedef std::shared_ptr<chinapres3ConditionDO> Ptrchinapres3ConditionDO;

#endif // !_chinapres3ConditionDO_H_