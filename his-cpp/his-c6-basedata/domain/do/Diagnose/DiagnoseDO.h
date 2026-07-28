//#pragma once
///*
// Copyright Zero One Star. All rights reserved.
// @Author: awei
// @Date: 2026/07/20 11:52:32
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//	  https://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*/
//#ifndef _DIAGNOSIS_DO_
//#define _DIAGNOSIS_DO_
//#include "../DoInclude.h"
///**
// * 诊断数据库实体DO，对应数据表 t_diagnosis
// */
//class DiagnosisDO : public BaseDO
//{
//	// 主键ID
//	MYSQL_SYNTHESIZE(string, id, Id);//主键id
//	// 诊断编码
//	MYSQL_SYNTHESIZE(string, diagnosisCode, DiagnosisCode);//诊断编码/特慢病编码 disease_code
//	// 诊断名称
//	MYSQL_SYNTHESIZE(string, diagnosisName, DiagnosisName);//诊断名称 name
//	// 诊断分类
//	MYSQL_SYNTHESIZE(string, diagnosisType, DiagnosisType);//诊断类型码 type_code
//	// 启用状态 0-停用 1-启用
//	MYSQL_SYNTHESIZE(string, status, Status);//状态 status_enum
//	// 备注
//	MYSQL_SYNTHESIZE(string, remark, Remark);//description
//	// 创建人
//	MYSQL_SYNTHESIZE(string, createBy, CreateBy);//created_by
//	// 创建时间
//	MYSQL_SYNTHESIZE(string, createTime, CreateTime);//created_at
//	// 修改人
//	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);//updated_by
//	// 修改时间
//	MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);//updated_at
//	//删除人
//	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);//deleted_by
//	//删除时间
//	MYSQL_SYNTHESIZE(string, deletedTime, DeletedTime);//deleted_at
//	// 医保编码
//	MYSQL_SYNTHESIZE(string, medicalInsuranceCode, MedicalInsuranceCode);//yb_no
//	// 拼音助记码
//	MYSQL_SYNTHESIZE(string, pinyinCode, PinyinCode);//py_flag
//	// 五笔助记码
//	MYSQL_SYNTHESIZE(string, wubiCode, WubiCode);//wb_flag
//	// 医保对码 0-F 1-T
//	MYSQL_SYNTHESIZE(int8_t, medicalInsuranceMatch, MedicalInsuranceMatch);//yb_match_flag
//	// 医保标记 0-F 1-T
//	MYSQL_SYNTHESIZE(int8_t, medicalInsuranceTag, MedicalInsuranceTag);//yb_flag
//	// 删除标记 0-F 1-T
//	MYSQL_SYNTHESIZE(int8_t, isDelete, IsDelete);// is_deleted
//	// 诊断目录
//	MYSQL_SYNTHESIZE(string, catName, CatName);//condition_code
//	//租户标识
//	MYSQL_SYNTHESIZE(string, tenantId, TenantId);//tenant_id
//	//乐观锁版本
//	MYSQL_SYNTHESIZE(int, version, Version);//version
//	//来源枚举值
//	MYSQL_SYNTHESIZE(string, source_enum, Source_enum);//source_enum
//public:
//	DiagnosisDO() : BaseDO("cln_condition_definition")
//	{
//		// 主键ID
//		MYSQL_ADD_FIELD_PK("id", "s", id);
//		// 诊断编码 disease_code
//		MYSQL_ADD_FIELD("disease_code", "s", diagnosisCode);
//		// 诊断名称 name
//		MYSQL_ADD_FIELD("name", "s", diagnosisName);
//		// 诊断分类 type_code
//		MYSQL_ADD_FIELD("type_code", "s", diagnosisType);
//		// 启用状态 status_enum
//		MYSQL_ADD_FIELD("status_enum", "s", status);
//		// 备注 description
//		MYSQL_ADD_FIELD("description", "s", remark);
//		// 创建人 created_by
//		MYSQL_ADD_FIELD("created_by", "s", createBy);
//		// 创建时间 created_at
//		MYSQL_ADD_FIELD("created_at", "s", createTime);
//		// 修改人 updated_by
//		MYSQL_ADD_FIELD("updated_by", "s", updateBy);
//		// 修改时间 updated_at
//		MYSQL_ADD_FIELD("updated_at", "s", updateTime);
//		// 删除人 deleted_by
//		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
//		// 删除时间 deleted_at
//		MYSQL_ADD_FIELD("deleted_at", "s", deletedTime);
//		// 医保编码 yb_no
//		MYSQL_ADD_FIELD("yb_no", "s", medicalInsuranceCode);
//		// 拼音助记码 py_flag
//		MYSQL_ADD_FIELD("py_flag", "s", pinyinCode);
//		// 五笔助记码 wb_flag
//		MYSQL_ADD_FIELD("wb_flag", "s", wubiCode);
//		// 医保对码 yb_match_flag 
//		MYSQL_ADD_FIELD("yb_match_flag", "b", medicalInsuranceMatch);
//		// 医保标记 yb_flag 
//		MYSQL_ADD_FIELD("yb_flag", "b", medicalInsuranceTag);
//		// 删除标记 is_deleted 
//		MYSQL_ADD_FIELD("is_deleted", "b", isDelete);
//		// 诊断目录 condition_code
//		MYSQL_ADD_FIELD("condition_code", "s", catName);
//		// 租户标识 tenant_id
//		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
//		//乐观锁版本version
//		MYSQL_ADD_FIELD("version", "i", version);
//		//来源枚举值
//		MYSQL_ADD_FIELD("source_enum", "i", source_enum);
//
//
//	}
//};
//typedef std::shared_ptr<DiagnosisDO> PtrDiagnosisDO;
//#endif // !_DIAGNOSIS_DO_