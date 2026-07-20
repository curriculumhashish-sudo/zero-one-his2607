
#ifndef _PATIENT_QUERY_
#define _PATIENT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者分页/查询参数
 */
class PatientQuery : public PageQuery {
    DTO_INIT(PatientQuery, PageQuery);
    // 姓名
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("patient.field.name"); }
    // 编号
    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("patient.field.id"); }
    // 性别
    DTO_FIELD(String, sex);
    DTO_FIELD_INFO(sex) { info->description = ZH_WORDS_GETTER("patient.field.sex"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PATIENT_QUERY_