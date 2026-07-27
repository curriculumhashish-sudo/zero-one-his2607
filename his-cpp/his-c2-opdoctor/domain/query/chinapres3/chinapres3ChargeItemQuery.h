#pragma once
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 中药处方收费项目分页查询条件传输对象
 *
 * 封装中药处方收费记录列表查询所需的过滤条件，继承分页查询基类
 * 继承 PageQuery 自动携带分页参数：页码 pageIndex、每页条数 pageSize
 *
 * 可关联处方单据，通过 serviceTable + serviceId 匹配对应的中药处方主记录
 */
class chinapres3ChargeItemQuery : public PageQuery
{
    DTO_INIT(chinapres3ChargeItemQuery, PageQuery);

    // 关联业务表名，固定传 "med_medication_request" 代表中药处方单据
    DTO_FIELD(String, serviceTable);
    DTO_FIELD_INFO(serviceTable) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.serviceTable");
    }

    // 业务单据ID，对应 serviceTable 指向的中药处方主键ID
    DTO_FIELD(String, serviceId);
    DTO_FIELD_INFO(serviceId) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.serviceId");
    }

    // 中药处方流水编号
    DTO_FIELD(String, prescriptionNo);
    DTO_FIELD_INFO(prescriptionNo) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.prescriptionNo");
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
};

#include OATPP_CODEGEN_END(DTO)