#pragma once
#ifndef _PAT_SELECT_DTO_H_
#define _PAT_SELECT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 弹窗选择患者单行数据（精简字段，适配弹窗表格）
 */
class PatSelectDTO : public oatpp::DTO
{
	DTO_INIT(PatSelectDTO, DTO);

	DTO_FIELD(String, busNo);		// 病历号
	DTO_FIELD_INFO(busNo) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.medicalNo");
	}

	DTO_FIELD(String, name);			// 姓名
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.name");
	}

	DTO_FIELD(String, gender);			// 性别 1男 2女
	DTO_FIELD_INFO(gender) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.gender");
	}

	DTO_FIELD(String, birthDate);		// 出生日期
	DTO_FIELD_INFO(birthDate) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.birthDay");
	}

	DTO_FIELD(Int32, age);				// 年龄
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.age");
	}

	DTO_FIELD(String, phone);			// 联系电话
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.phone");
	}

	DTO_FIELD(String, idCard);			// 身份证号
	DTO_FIELD_INFO(idCard) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.idCard");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_SELECT_DTO_H_