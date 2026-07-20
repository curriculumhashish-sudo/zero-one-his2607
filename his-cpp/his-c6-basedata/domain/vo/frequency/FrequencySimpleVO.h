#pragma once
#ifndef _FREQUENCYSIMPLEVO_H_
#define _FREQUENCYSIMPLEVO_H_

#include "../../GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 频次名称
 */
class FrequencySimpleVO : public JsonVO
{
	DTO_INIT(FrequencySimpleVO, JsonVO);

	// 主键ID（项目String）
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("common.id"), true);
	// 频次名称
	API_DTO_FIELD_REQUIRE(String, freqName, ZH_WORDS_GETTER("frequency.field.freqName"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYSIMPLEVO_H_