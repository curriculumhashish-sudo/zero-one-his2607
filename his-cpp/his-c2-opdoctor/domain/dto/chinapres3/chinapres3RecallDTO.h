#pragma once

#ifndef _chinapres3RECALLDTO_H_
#define _chinapres3RECALLDTO_H_
#include "../../GlobalInclude.h"
// #include "chinapres3ItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 撤回处方DTO
 */
class chinapres3RecallDTO : public oatpp::DTO
{
	DTO_INIT(chinapres3RecallDTO, DTO);

	// 处方ID
	API_DTO_FIELD_REQUIRE(
		String,
		chinapres3Id,
		ZH_WORDS_GETTER("chinapres3.field.chinapres3Id"),
		true);
};
#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3RecallDTO_H_#pragma once
