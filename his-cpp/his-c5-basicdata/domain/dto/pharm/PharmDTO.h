#pragma once

#ifndef _PHARM_DTO_
#define _PHARM_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmDTO : public oatpp::DTO
{
	// 初始化定义
	DTO_INIT(PharmDTO, DTO);
};

#include OATPP_CODEGEN_END(DTO)

#endif	// !_PHARM_DTO_