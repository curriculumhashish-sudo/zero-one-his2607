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
 */
class DiagnosisDO : public BaseDO
{
	// 主键ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 诊断编码
	MYSQL_SYNTHESIZE(string, diagnosisCode, DiagnosisCode);
	// 诊断名称
	MYSQL_SYNTHESIZE(string, diagnosisName, DiagnosisName);
	// 诊断分类
	MYSQL_SYNTHESIZE(string, diagnosisType, DiagnosisType);//你看一下参考项目，这个是有三个类型，都是string类型的
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
	// 医保编码
	MYSQL_SYNTHESIZE(string, medicalInsuranceCode, MedicalInsuranceCode);
	// 诊断目录id外键
	MYSQL_SYNTHESIZE(long long, categoryId, CategoryId);
	// 拼音助记码
	MYSQL_SYNTHESIZE(string, pinyinCode, PinyinCode);
	// 五笔助记码
	MYSQL_SYNTHESIZE(string, wubiCode, WubiCode);
	// 医保对码 0-F 1-T
	MYSQL_SYNTHESIZE(int, medicalInsuranceMatch, MedicalInsuranceMatch);
	// 医保标记 0-F 1-T
	MYSQL_SYNTHESIZE(int, medicalInsuranceTag, MedicalInsuranceTag);
	// 删除标记 0-F 1-T
	MYSQL_SYNTHESIZE(int, isDelete, IsDelete);
	// 分类项目名
	MYSQL_SYNTHESIZE(string, catName, CatName);
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
		MYSQL_ADD_FIELD("medical_insurance_code", "s", medicalInsuranceCode);
		MYSQL_ADD_FIELD("category_id", "l", categoryId);
		MYSQL_ADD_FIELD("pinyin_code", "s", pinyinCode);
		MYSQL_ADD_FIELD("wubi_code", "s", wubiCode);
		MYSQL_ADD_FIELD("medical_insurance_match", "i", medicalInsuranceMatch);
		MYSQL_ADD_FIELD("medical_insurance_tag", "i", medicalInsuranceTag);
		MYSQL_ADD_FIELD("is_delete", "i", isDelete);
		MYSQL_ADD_FIELD("cat_name", "s", catName);
	}
};
typedef std::shared_ptr<DiagnosisDO> PtrDiagnosisDO;
#endif // !_DIAGNOSIS_DO_