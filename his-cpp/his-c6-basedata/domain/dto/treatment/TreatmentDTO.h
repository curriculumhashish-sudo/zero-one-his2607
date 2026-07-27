#pragma once

#ifndef _TREATMENTDTO_H_
#define _TREATMENTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 列表分页行数据
class TreatmentDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentDTO, DTO);

	// 主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
	// 编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("treatment.field.code"), true);
	// 项目名
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
	// 状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("treatment.field.status"), true);
	// 分类
	API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("treatment.field.classification"), true);
	// 价格
	API_DTO_FIELD_REQUIRE(String, price, ZH_WORDS_GETTER("treatment.field.price"), true);

};

#include OATPP_CODEGEN_END(DTO)

#endif //！_TREATMENTDTO_H_
