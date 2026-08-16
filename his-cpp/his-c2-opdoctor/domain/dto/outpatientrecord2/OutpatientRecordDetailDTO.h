#ifndef __OUTPATIENTRECORDDETAILDTO_H_
#define __OUTPATIENTRECORDDETAILDTO_H_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientRecordDetailDTO : public oatpp::DTO
{
    DTO_INIT(OutpatientRecordDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("outpatientRecord.field.id"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("outpatientRecord.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("outpatientRecord.field.content"), false);
    API_DTO_FIELD_REQUIRE(String, updatedAt, ZH_WORDS_GETTER("outpatientRecord.field.updatedAt"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif