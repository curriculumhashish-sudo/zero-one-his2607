#pragma once

#ifndef _DEPT_DTO_
#define _DEPT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DeptAddDTO : public oatpp::DTO
{
	DTO_INIT(DeptAddDTO, DTO);

	// 科室名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("dept.field.name"), true)
	// 类型
	API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("dept.field.type-enum"), false)
	// 类别
	API_DTO_FIELD_REQUIRE(String, classEnum, ZH_WORDS_GETTER("dept.field.class-enum"), false)
	// 上级组织编号
	API_DTO_FIELD_REQUIRE(String, parentId, ZH_WORDS_GETTER("dept.field.parent-id"), false)
	// 医保编码
	API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("dept.field.yb-no"), false)
	// 医保名称
	API_DTO_FIELD_REQUIRE(String, ybName, ZH_WORDS_GETTER("dept.field.yb-name"), false)
	// 机构标识
	API_DTO_FIELD_REQUIRE(String, medinsId, ZH_WORDS_GETTER("dept.field.medins-id"), false)
	// 医保区划
	API_DTO_FIELD_REQUIRE(String, medinsAdmdvs, ZH_WORDS_GETTER("dept.field.medins-admdvs"), false)
	// 机构类型
	API_DTO_FIELD_REQUIRE(String, medinsType, ZH_WORDS_GETTER("dept.field.medins-type"), false)
	// 机构等级
	API_DTO_FIELD_REQUIRE(String, medinsLv, ZH_WORDS_GETTER("dept.field.medins-lv"), false)
	// 拼音码
	API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("dept.field.py-str"), false)
	// 五笔码
	API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("dept.field.wb-str"), false)
	// 默认医生编号
	API_DTO_FIELD_REQUIRE(String, defDoctorId, ZH_WORDS_GETTER("dept.field.def-doctor-id"), false)
	// 科别
	API_DTO_FIELD_REQUIRE(String, caty, ZH_WORDS_GETTER("dept.field.caty"), false)
	// 显示顺序
	API_DTO_FIELD_REQUIRE(String, displayOrder, ZH_WORDS_GETTER("dept.field.display-order"), false)
	// 状态
	API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("dept.field.status-enum"), false)
	// 是否启用
	API_DTO_FIELD_REQUIRE(String, activeFlag, ZH_WORDS_GETTER("dept.field.active-flag"), false)
	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
	std::string validate() const
	{
		if (!name || name->empty())
			return "name is required.";
		return "";
	}
};

class DeptModifyDTO : public DeptAddDTO
{
	DTO_INIT(DeptModifyDTO, DeptAddDTO);

	// 科室编号
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("dept.field.id"), true)

public:
	std::string validate() const
	{
		if (!id || id->empty())
			return "id is required.";
		return DeptAddDTO::validate();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DEPT_DTO_
