#pragma once
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 中药处方发药记录分页查询条件传输对象
 *
 * 封装中药处方发药记录列表过滤条件，可通过处方ID、患者ID等条件筛选数据
 * 继承 PageQuery 自动携带分页参数：页码 pageIndex、每页条数 pageSize
 */
class chinapres3MedicationDispenseQuery : public PageQuery
{
    DTO_INIT(chinapres3MedicationDispenseQuery, PageQuery);

    // 处方单据ID，关联中药处方主表 med_medication_request.id
    DTO_FIELD(String, medReqId);
    DTO_FIELD_INFO(medReqId) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.medReqId");
    }

    // 患者唯一标识ID
    DTO_FIELD(String, patientId);
    DTO_FIELD_INFO(patientId) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.patientId");
    }

    // 就诊记录唯一标识ID
    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.encounterId");
    }

    // 发药业务全局唯一流水号
    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.busNo");
    }

    // 发药状态枚举：PREPARATION 待调配 / DISPENSED 已发药
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)