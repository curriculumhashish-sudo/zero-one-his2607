#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtaDiagnoseAddDTOin a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _DIAGNOSE_DTO_H_
#define _DIAGNOSE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)


//增加诊断条目
class DiagnoseAddDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseAddDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("diagnose.field.diag.name"), true);//诊断名称（非空） name
	API_DTO_FIELD_DEFAULT(String, diagType, ZH_WORDS_GETTER("diagnose.field.diag.diag_type"));//诊断类型码   type_code
	API_DTO_FIELD_DEFAULT(String, chronicCode, ZH_WORDS_GETTER("diagnose.field.diag.chronic_code"));//诊断编码/特慢病编码 disease_code
	API_DTO_FIELD_DEFAULT(String, medicalInsuranceCode, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_code"));//医保编码 condition_code 对应医保编码
	API_DTO_FIELD_DEFAULT(String, status, ZH_WORDS_GETTER("diagnose.field.diag.status"));//状态枚举值【非空】（ACTIVE/STOP）  status_enum
	API_DTO_FIELD_DEFAULT(Int32, medicalInsuranceTag, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_tag"));//医保标签 yb_flag
	API_DTO_FIELD_DEFAULT(Int32, medicalInsuranceMatch, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_match"));//医保对码 yb_match_flag
	API_DTO_FIELD_DEFAULT(String, pinyinCode, ZH_WORDS_GETTER("diagnose.field.diag.pinyin_code"));//拼音助记  py_str
	API_DTO_FIELD_DEFAULT(String, wubiCode, ZH_WORDS_GETTER("diagnose.field.diag.wubi_code"));//五笔助记 wb_str
	//API_DTO_FIELD_DEFAULT(Int32, isDelete, ZH_WORDS_GETTER("diagnose.field.diag.is_delete"));//【非空】逻辑删除 is_deleted
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("diagnose.field.diag.remark"));//描述详情 description
	API_DTO_FIELD_REQUIRE(String, catName, ZH_WORDS_GETTER("diagnose.field.category.cat_name"), true);//诊断目录/病情编码（非空）对应source_enum是获得诊断目录的
	//API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("diagnose.field.diag.tenant_id"), true);//租户标识 tenant_id
	//API_DTO_FIELD_REQUIRE(String, source_enum, ZH_WORDS_GETTER("diagnose.field.diag.source_enum"), true);//来源枚举值 source_enum对应诊断目录

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);//请求头token指针

public:
	std::string validate()
	{
		if (!name || name->empty())
		{
			return "The disease name cannot be empty";
		}
		if (!catName || catName->empty())
		{
			return "The category name cannot be empty";
		}

		//if (diagType && !diagType->empty()
		//	&& *diagType != "1"//western_medicine_diagnose
		//	&& *diagType != "2"//chinese_main_disease_diagnose
		//	&& *diagType != "3"//chinese_main_symptom_diagnose
		//	)
		//{
		//	return "The diagnose type is invalid";
		//}

		if (!status->empty() && *status != "ACTIVE" && *status != "STOP")
		{
			return "The status only allows ACTIVE or STOP";
		}
		if (medicalInsuranceTag && *medicalInsuranceTag != 0 && *medicalInsuranceTag != 1)
		{
			return "The medical insurance flag only allows 0 or 1";
		}
		if (medicalInsuranceMatch && *medicalInsuranceMatch != 0 && *medicalInsuranceMatch != 1)
		{
			return "The medical insurance match flag only allows 0 or 1";
		}
		//if (isDelete && *isDelete != 0 && *isDelete != 1)
		//{
		//	return "The delete flag only allows 0 or 1";
		//}

		return "";
	}
};

//修改诊断条目
class DiagnoseDTO : public DiagnoseAddDTO
{
	DTO_INIT(DiagnoseDTO, DiagnoseAddDTO);

	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.diag.id"), true);//主键id  id
	API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("diagnose.field.diag.version"), true);

};

//停/启用诊断条目
class DiagnoseBatchDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseBatchDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.diag.id_list"), true);//ids可以看作指针
	API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("diagnose.field.diag.version"), true);//乐观锁版本


	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

class DiagnoseBatchStatusDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseBatchStatusDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<DiagnoseBatchDTO::Wrapper>, ids, ZH_WORDS_GETTER("diagnose.field.diag.id_list"), true);//ids可以看作指针
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("diagnose.field.diag.status"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

//逻辑删除诊断条目
class DiagnoseRemoveDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseRemoveDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("diagnose.field.diag.id_list"), true);
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSE_DTO_H_
