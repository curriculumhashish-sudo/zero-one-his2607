#pragma once

#ifndef _DEPT_DTO_
#define _DEPT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DeptAddDTO : public oatpp::DTO
{
	DTO_INIT(DeptAddDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("dept.field.name"), true)

	API_DTO_FIELD_REQUIRE(String, typeEnum, ZH_WORDS_GETTER("dept.field.type-enum"), false)

	API_DTO_FIELD_REQUIRE(String, classEnum, ZH_WORDS_GETTER("dept.field.class-enum"), false)

	API_DTO_FIELD_REQUIRE(String, parentId, ZH_WORDS_GETTER("dept.field.parent-id"), false)

	API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("dept.field.yb-no"), false)

	API_DTO_FIELD_REQUIRE(String, ybName, ZH_WORDS_GETTER("dept.field.yb-name"), false)

	API_DTO_FIELD_REQUIRE(String, medinsId, ZH_WORDS_GETTER("dept.field.medins-id"), false)

	API_DTO_FIELD_REQUIRE(String, medinsAdmdvs, ZH_WORDS_GETTER("dept.field.medins-admdvs"), false)

	API_DTO_FIELD_REQUIRE(String, medinsType, ZH_WORDS_GETTER("dept.field.medins-type"), false)

	API_DTO_FIELD_REQUIRE(String, medinsLv, ZH_WORDS_GETTER("dept.field.medins-lv"), false)

	API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("dept.field.py-str"), false)

	API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("dept.field.wb-str"), false)

	API_DTO_FIELD_REQUIRE(String, defDoctorId, ZH_WORDS_GETTER("dept.field.def-doctor-id"), false)

	API_DTO_FIELD_REQUIRE(String, caty, ZH_WORDS_GETTER("dept.field.caty"), false)

	API_DTO_FIELD_REQUIRE(String, displayOrder, ZH_WORDS_GETTER("dept.field.display-order"), false)

	API_DTO_FIELD_REQUIRE(String, statusEnum, ZH_WORDS_GETTER("dept.field.status-enum"), false)

	API_DTO_FIELD_REQUIRE(String, activeFlag, ZH_WORDS_GETTER("dept.field.active-flag"), false)

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
