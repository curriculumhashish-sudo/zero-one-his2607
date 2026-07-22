#pragma once

#ifndef _chinapres3ISSUEDTO_H_
#define _chinapres3ISSUEDTO_H_

#include "../../GlobalInclude.h"
#include "chinapres3ItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 签发处方DTO
 */
class chinapres3IssueDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3IssueDTO, DTO);

    // 患者ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("chinapres3.field.patientId"), true);

    // 诊断ID
    API_DTO_FIELD_REQUIRE(String, diagnosisId, ZH_WORDS_GETTER("chinapres3.field.diagnosisId"), true);

    // 用法
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("chinapres3.field.usage"), true);

    // 频次
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("chinapres3.field.frequency"), true);

    // 天数
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("chinapres3.field.days"), true);

    // 付数
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("chinapres3.field.copies"), true);

    // 是否代煎
    API_DTO_FIELD_REQUIRE(Boolean, decoction, ZH_WORDS_GETTER("chinapres3.field.decoction"), true);

    // 医嘱备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("chinapres3.field.remark"), false);

	// 添加药材
	API_DTO_FIELD_DEFAULT(
		List<chinapres3ItemDTO::Wrapper>,
		items,
		ZH_WORDS_GETTER("chinapres3.field.items"),true);

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3ISSUEDTO_H_#pragma once
