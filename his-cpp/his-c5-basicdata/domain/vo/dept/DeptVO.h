#pragma once

#ifndef _DEPT_VO_
#define _DEPT_VO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DeptAddJsonVO : public JsonVO<oatpp::String>
{
	DTO_INIT(DeptAddJsonVO, JsonVO<oatpp::String>);
};

class DeptModifyJsonVO : public JsonVO<oatpp::String>
{
	DTO_INIT(DeptModifyJsonVO, JsonVO<oatpp::String>);
};

class DeptDeleteJsonVO : public JsonVO<oatpp::String>
{
	DTO_INIT(DeptDeleteJsonVO, JsonVO<oatpp::String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DEPT_VO_
