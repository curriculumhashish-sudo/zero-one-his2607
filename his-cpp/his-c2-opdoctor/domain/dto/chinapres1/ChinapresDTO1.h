#pragma once
#ifndef _CHINAPRESDTO1_H_
#define _CHINAPRESDTO1_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresDTO1 : public oatpp::DTO
{
	DTO_INIT(ChinapresDTO1, DTO);
	// 处方号
	API_DTO_FIELD_REQUIRE(String, prescriptionNo, ZH_WORDS_GETTER("chinapres1.field.prescriptionno"), true);
	//id
	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("chinapres1.field.id"), false, "");
	//就诊号
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("chinapres1.field.encounterid"), false, "");
	// 药材摘要（字符串，如"当归,黄芪"）
	API_DTO_FIELD(String, summary, ZH_WORDS_GETTER("chinapres1.field.summary"), false, "");
	// 药材名数组 ["当归","黄芪"]
	API_DTO_FIELD(List<String>, herbNames, ZH_WORDS_GETTER("chinapres1.field.herbnames"), false, nullptr);
	// 开方时间
	API_DTO_FIELD(String, authoredAt, ZH_WORDS_GETTER("chinapres1.field.authoredat"), false, "");
};

#include OATPP_CODEGEN_END(DTO)

#endif