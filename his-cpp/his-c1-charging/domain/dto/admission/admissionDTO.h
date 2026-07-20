#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 10:59:38

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
#ifndef _SAMPLE_DTO_
#define _SAMPLE_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 示例新增数据传输对象
 */
class AdmissionAddDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionAddDTO, DTO);
	// 姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.name");
	}

	//身份证号
	DTO_FIELD(Int64, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//出生日期
	DTO_FIELD(String, birthtime);
	DTO_FIELD_INFO(birthtime) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	// 年龄
	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.age");
	}

	// 性别
	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.sex");
	}

	//联系方式
	DTO_FIELD(Int64, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院科室
	DTO_FIELD(String, admissionDept);
	DTO_FIELD_INFO(admissionDept) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院病区
	DTO_FIELD(String, admission_ward);
	DTO_FIELD_INFO(admission_ward) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//主治医师
	DTO_FIELD(String, attending_doctor);
	DTO_FIELD_INFO(attending_doctor) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院诊断
	DTO_FIELD(String, admissionDiagnosis);
	DTO_FIELD_INFO(admissionDiagnosis) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//诊断描述
	DTO_FIELD(String, diagnosisDesc);
	DTO_FIELD_INFO(diagnosisDesc) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//患者病情
	DTO_FIELD(String, patientCondition);
	DTO_FIELD_INFO(patientCondition) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院类型
	DTO_FIELD(String, admissionType);
	DTO_FIELD_INFO(admissionType) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院方式
	DTO_FIELD(String, admissionMode);
	DTO_FIELD_INFO(admissionMode) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//费用性质
	DTO_FIELD(String, feeNature);
	DTO_FIELD_INFO(feeNature) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	//入院日期
	DTO_FIELD(String, admissiondata);
	DTO_FIELD_INFO(admissiondata) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.id");
	}

	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 数据校验
	std::string validate()
	{
		if (!age || age < 0)
			return "age invalidate.";
		if (!name || name->empty())
			return "name invalidate.";
		if (!sex || sex->empty())
			return "sex invalidate.";
		return "";
	}
};

class AdmissionDeleteDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionDeleteDTO, DTO);
	// 姓名
	DTO_FIELD(String, thedelete);
	DTO_FIELD_INFO(thedelete) {
		info->description = ZH_WORDS_GETTER("yiguang.charging.admission.delete");
	}

	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	std::string validate()
	{
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_DTO_