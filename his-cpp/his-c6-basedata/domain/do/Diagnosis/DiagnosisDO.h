#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:52:32

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _DIAGNOSIS_DO_
#define _DIAGNOSIS_DO_
#include "../DoInclude.h"

/**
 * 诊断数据库实体DO，对应数据表 t_diagnosis
 * 练手精简版，仅覆盖页面所需字段
 */
class DiagnosisDO : public BaseDO
{
	// 主键ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 诊断编码
	MYSQL_SYNTHESIZE(string, diagnosisCode, DiagnosisCode);
	// 诊断名称
	MYSQL_SYNTHESIZE(string, diagnosisName, DiagnosisName);
	// 诊断分类 1-西医 2-中医
	MYSQL_SYNTHESIZE(int, diagnosisType, DiagnosisType);
	// 启用状态 0-停用 1-启用
	MYSQL_SYNTHESIZE(int, status, Status);
	// 备注
	MYSQL_SYNTHESIZE(string, remark, Remark);
	// 创建人
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createTime, CreateTime);
	// 修改人
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// 修改时间
	MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);
public:
	DiagnosisDO() : BaseDO("t_diagnosis")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("diagnosis_code", "s", diagnosisCode);
		MYSQL_ADD_FIELD("diagnosis_name", "s", diagnosisName);
		MYSQL_ADD_FIELD("diagnosis_type", "i", diagnosisType);
		MYSQL_ADD_FIELD("status", "i", status);
		MYSQL_ADD_FIELD("remark", "s", remark);
		MYSQL_ADD_FIELD("create_by", "s", createBy);
		MYSQL_ADD_FIELD("create_time", "s", createTime);
		MYSQL_ADD_FIELD("update_by", "s", updateBy);
		MYSQL_ADD_FIELD("update_time", "s", updateTime);
	}
};

typedef std::shared_ptr<DiagnosisDO> PtrDiagnosisDO;
#endif // !_DIAGNOSIS_DO_