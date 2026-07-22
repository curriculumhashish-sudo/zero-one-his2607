#pragma once
#ifndef _PATIENT_DTO_
#define _PATIENT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者新增数据传输对象
 */
class PatientAddDTO : public oatpp::DTO
{
	DTO_INIT(PatientAddDTO, DTO);

	// 患者业务编号
	DTO_FIELD(String, busNo);
	DTO_FIELD_INFO(busNo) {
		info->description = ZH_WORDS_GETTER("patient.field.id");
	}

	// 患者姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("patient.field.name");
	}

	// 姓名扩展JSON
	DTO_FIELD(String, nameJson);

	// 性别编码
	DTO_FIELD(String, genderEnum);

	// 出生日期
	DTO_FIELD(String, birthDate);

	// 患者状态
	DTO_FIELD(String, statusEnum);

	// 启用标记
	DTO_FIELD(Boolean, activeFlag);


	// 临时建档标记
	DTO_FIELD(Boolean, tempFlag);


	// 身份证号
	DTO_FIELD(String, idCard);

	// 婚姻状态
	DTO_FIELD(String, maritalStatusEnum);

	// 职业编码
	DTO_FIELD(String, prfsEnum);

	// 联系电话
	DTO_FIELD(String, phone);

	// 地址
	DTO_FIELD(String, address);
	DTO_FIELD(String, addressProvince);
	DTO_FIELD(String, addressCity);
	DTO_FIELD(String, addressDistrict);
	DTO_FIELD(String, addressStreet);

	// 地址扩展
	DTO_FIELD(String, addressExtendJson);

	// 民族编码
	DTO_FIELD(String, nationalityCode);

	// 拼音码
	DTO_FIELD(String, pyStr);
	// 五笔码
	DTO_FIELD(String, wbStr);


	// ABO血型
	DTO_FIELD(Int32, bloodTypeAbo);
	// RH血型
	DTO_FIELD(Int32, bloodTypeRh);

	// 工作单位
	DTO_FIELD(String, workCompany);

	// 籍贯
	DTO_FIELD(String, nativePlace);
	// 国家地区
	DTO_FIELD(String, countryCode);


	// 联系人姓名
	DTO_FIELD(String, linkName);
	// 联系关系
	DTO_FIELD(Int32, linkRelationCode);
	// 联系电话
	DTO_FIELD(String, linkTelcom);
	// 联系人JSON
	DTO_FIELD(String, linkJsons);


	// 建档机构
	DTO_FIELD(String, organizationId);

	// 校验
	std::string validate()
	{
		if (!name || name->empty())
			return "patient name invalidate.";

		if (!genderEnum || genderEnum->empty())
			return "gender invalidate.";

		if (!organizationId || organizationId->empty())
			return "organization invalidate.";

		return "";
	}
};


/**
 * 患者完整传输对象
 */
class PatientDTO : public PatientAddDTO
{
	DTO_INIT(PatientDTO, PatientAddDTO);

	// 主键
	DTO_FIELD(String, id);
	// 租户
	DTO_FIELD(String, tenantId);


	// 创建时间
	DTO_FIELD(String, createdAt);
	// 更新时间
	DTO_FIELD(String, updatedAt);
	// 创建人
	DTO_FIELD(String, createdBy);
	// 更新人
	DTO_FIELD(String, updatedBy);


	// 删除标记
	DTO_FIELD(Boolean, isDeleted);

	// 版本号
	DTO_FIELD(Int32, version);

	// 死亡时间
	DTO_FIELD(String, deceasedDate);
	// 死亡原因
	DTO_FIELD(String, deceasedReason);


	// 合并目标患者
	DTO_FIELD(String, mergedTargetPatientId);

	// 合并时间
	DTO_FIELD(String, mergedAt);
};


/**
 * 患者修改DTO
 */
class PatientUpdateDTO : public oatpp::DTO
{
	DTO_INIT(PatientUpdateDTO, DTO);

	// 患者ID
	DTO_FIELD(String, id);

	// 患者姓名
	DTO_FIELD(String, name);

	// 联系电话
	DTO_FIELD(String, phone);

	// 地址
	DTO_FIELD(String, address);
	DTO_FIELD(String, addressProvince);
	DTO_FIELD(String, addressCity);
	DTO_FIELD(String, addressDistrict);
	DTO_FIELD(String, addressStreet);


	// 婚姻状态
	DTO_FIELD(String, maritalStatusEnum);

	// 职业
	DTO_FIELD(String, prfsEnum);

	// 联系人
	DTO_FIELD(String, linkName);
	DTO_FIELD(Int32, linkRelationCode);
	DTO_FIELD(String, linkTelcom);

	std::string validate()
	{
		if (!id || id->empty())
			return "patient id invalidate.";

		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_DTO_