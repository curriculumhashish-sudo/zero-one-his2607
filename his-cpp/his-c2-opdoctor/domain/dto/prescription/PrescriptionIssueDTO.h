#pragma once

#ifndef _PRESCRIPTIONISSUEDTO_H_
#define _PRESCRIPTIONISSUEDTO_H_

#include "../../GlobalInclude.h"
#include "PrescriptionItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 签发处方DTO
 */
class PrescriptionIssueDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionIssueDTO, DTO);

    // 患者ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"), true);

    // 诊断ID
    API_DTO_FIELD_REQUIRE(String, diagnosisId, ZH_WORDS_GETTER("prescription.field.diagnosisId"), true);

    // 用法
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("prescription.field.usage"), true);

    // 频次
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("prescription.field.frequency"), true);

    // 天数
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("prescription.field.days"), true);

    // 付数
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("prescription.field.copies"), true);

    // 是否代煎
    API_DTO_FIELD_REQUIRE(Boolean, decoction, ZH_WORDS_GETTER("prescription.field.decoction"), true);

    // 医嘱备注
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("prescription.field.remark"), false);

	// 添加药材
	API_DTO_FIELD_DEFAULT(
		List<PrescriptionItemDTO::Wrapper>,
		items,
		ZH_WORDS_GETTER("prescription.field.items"),true);

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTIONISSUEDTO_H_#pragma once
