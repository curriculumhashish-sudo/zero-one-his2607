#pragma once

#ifndef _outpatientrecordSAVEDTO_H_
#define _outpatientrecordSAVEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 门诊病历保存DTO
 */
class outpatientrecordSaveDTO : public oatpp::DTO
{
    DTO_INIT(outpatientrecordSaveDTO, DTO);

    // 病历ID（为空表示新增，不为空表示修改）
    API_DTO_FIELD_REQUIRE(
        String,
        outpatientrecordId,
        ZH_WORDS_GETTER("outpatientrecord.field.outpatientrecordId"),
        false);

    // 患者ID
    API_DTO_FIELD_REQUIRE(
        String,
        patientId,
        ZH_WORDS_GETTER("outpatientrecord.field.patientId"),
        true);

    // 就诊ID（如果系统没有，可先去掉）
   /* API_DTO_FIELD_REQUIRE(
		String,
		visitId,
		ZH_WORDS_GETTER("outpatientrecord.field.visitId"),
		false);*/

    // 病历标题
   /* API_DTO_FIELD_REQUIRE(
        String,
        title,
        ZH_WORDS_GETTER("outpatientrecord.field.title"),
        false);*/

    // 病历正文
    API_DTO_FIELD_REQUIRE(
        String,
        content,
        ZH_WORDS_GETTER("outpatientrecord.field.content"),
        true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_outpatientrecordSAVEDTO_H_