#ifndef __OUTPATIENTRECORDDTO_H_
#define __OUTPATIENTRECORDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientRecordDTO : public oatpp::DTO
{
    DTO_INIT(OutpatientRecordDTO, DTO);

    // 🆕 显式提供 encounterId（与 id 值相同，便于前端语义理解）
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("outpatientRecord.field.encounterId"), true);
    // 就诊日期（使用 start_time）
    API_DTO_FIELD_REQUIRE(String, visitDate, ZH_WORDS_GETTER("outpatientRecord.field.visitDate"), true);
    // 诊断（取主诊断 description）
    API_DTO_FIELD_REQUIRE(String, diagnosis, ZH_WORDS_GETTER("outpatientRecord.field.diagnosis"), true);
    // 医生姓名（通过参与者获取）
    API_DTO_FIELD_REQUIRE(String, doctorName, ZH_WORDS_GETTER("outpatientRecord.field.doctorName"), false);
    // 科室名称（organization_id）
    API_DTO_FIELD_REQUIRE(String, deptName, ZH_WORDS_GETTER("outpatientRecord.field.deptName"), false);
};

class OutpatientRecordPageDTO : public PageDTO<OutpatientRecordDTO::Wrapper>
{
    DTO_INIT(OutpatientRecordPageDTO, PageDTO<OutpatientRecordDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !__OUTPATIENTRECORDDTO_H_