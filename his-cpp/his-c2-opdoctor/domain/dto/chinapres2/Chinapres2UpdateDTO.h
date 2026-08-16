#ifndef __CHINAPRES2UPDATEDTO_H_
#define __CHINAPRES2UPDATEDTO_H_

#include "../../GlobalInclude.h"
#include "Chinapres2DetailAddDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Chinapres2UpdateDTO : public oatpp::DTO {
    DTO_INIT(Chinapres2UpdateDTO, DTO);

    // 处方ID（必填）
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("chinapres2.field.id"), true);
    // 就诊ID（必填）
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("chinapres2.field.encounterId"), true);
    // 患者ID（必填）
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("chinapres2.field.patientId"), true);
    // 就诊诊断ID（必填）
    API_DTO_FIELD_REQUIRE(String, encounterDiagnosisId, ZH_WORDS_GETTER("chinapres2.field.encounterDiagnosisId"), true);
    // 用法（必填）
    API_DTO_FIELD_REQUIRE(String, usage, ZH_WORDS_GETTER("chinapres2.field.usage"), true);
    // 频次（必填）
    API_DTO_FIELD_REQUIRE(String, frequency, ZH_WORDS_GETTER("chinapres2.field.frequency"), true);
    // 天数（必填，正整数）
    API_DTO_FIELD_REQUIRE(Int32, days, ZH_WORDS_GETTER("chinapres2.field.days"), true);
    // 付数（必填，正整数）
    API_DTO_FIELD_REQUIRE(Int32, copies, ZH_WORDS_GETTER("chinapres2.field.copies"), true);
    // 是否代煎（可选）
    API_DTO_FIELD_REQUIRE(Int32, decoction, ZH_WORDS_GETTER("chinapres2.field.decoction"), false);
    // 备注（可选）
    API_DTO_FIELD_REQUIRE(String, remark, ZH_WORDS_GETTER("chinapres2.field.remark"), false);
    // 版本号（必填，乐观锁）
    API_DTO_FIELD_REQUIRE(Int64, version, ZH_WORDS_GETTER("chinapres2.field.version"), true);
    // 药材明细（必填）
    API_DTO_FIELD_REQUIRE(oatpp::List<Chinapres2DetailAddDTO::Wrapper>, details, ZH_WORDS_GETTER("chinapres2.field.details"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif