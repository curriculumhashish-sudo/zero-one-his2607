#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 并组医嘱请求DTO（含拆组/并组两阶段操作）
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _ORDERMERGEDTO_H_
#define _ORDERMERGEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 并组载荷
class MergeAssignGroupDTO : public oatpp::DTO
{
    DTO_INIT(MergeAssignGroupDTO, DTO);
    // 新并组编号（为空则自动生成）
    API_DTO_FIELD_DEFAULT(String, groupId, ZH_WORDS_GETTER("medicalorder.field.groupId"));
    // 需合并的医嘱ID列表
    API_DTO_FIELD_DEFAULT(List<String>, medicationRequestIds, ZH_WORDS_GETTER("medicalorder.field.orderIds"));
};

// 并组医嘱请求DTO
class OrderMergeDTO : public oatpp::DTO
{
    DTO_INIT(OrderMergeDTO, DTO);
    // 就诊ID
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("medicalorder.field.encounterId"));
    // 患者ID
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("medicalorder.field.patientId"));
    // 操作人ID
    API_DTO_FIELD_DEFAULT(String, operatorId, ZH_WORDS_GETTER("medicalorder.field.operatorId"));
    // 需清空的组ID列表（并组前先拆组）
    API_DTO_FIELD_DEFAULT(List<String>, clearGroupIds, ZH_WORDS_GETTER("medicalorder.field.clearGroupIds"));
    // 并组载荷
    API_DTO_FIELD_DEFAULT(Object<MergeAssignGroupDTO>, assignGroup, ZH_WORDS_GETTER("medicalorder.field.assignGroup"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
