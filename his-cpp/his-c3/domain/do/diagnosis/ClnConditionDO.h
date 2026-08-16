#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:52:32

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _CLN_CONDITION_DO_H_
#define _CLN_CONDITION_DO_H_
#include "../DoInclude.h"

/**
 * 临床病情 数据库实体类
 */
class ClnConditionDO : public BaseDO
{
    // 主键
    MYSQL_SYNTHESIZE(string, id, Id);
    // 租户标识
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    // 创建时间
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    // 最后更新时间
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    // 创建人
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    // 最后更新人
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    // 软删标记
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    // 删除时间
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    // 删除人
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    // 乐观锁版本号
    MYSQL_SYNTHESIZE(int, version, Version);
    // verification状态枚举值
    MYSQL_SYNTHESIZE(string, verificationStatusEnum, VerificationStatusEnum);
    // clinical状态枚举值
    MYSQL_SYNTHESIZE(string, clinicalStatusEnum, ClinicalStatusEnum);
    // category枚举值
    MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
    // severity枚举值
    MYSQL_SYNTHESIZE(string, severityEnum, SeverityEnum);
    // 业务编号
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    // 身体siteJSON
    MYSQL_SYNTHESIZE(string, bodySiteJson, BodySiteJson);
    // 身体结构标识
    MYSQL_SYNTHESIZE(string, bodyStructureId, BodyStructureId);
    // 患者标识
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    // 描述
    MYSQL_SYNTHESIZE(string, description, Description);
    // 发病时间
    MYSQL_SYNTHESIZE(string, onsetDatetime, OnsetDatetime);
    // 发病年龄
    MYSQL_SYNTHESIZE(int, onsetAge, OnsetAge);
    // onset文本
    MYSQL_SYNTHESIZE(string, onsetText, OnsetText);
    // 缓解时间
    MYSQL_SYNTHESIZE(string, abatementDatetime, AbatementDatetime);
    // 缓解年龄
    MYSQL_SYNTHESIZE(int, abatementAge, AbatementAge);
    // 缓解描述
    MYSQL_SYNTHESIZE(string, abatementString, AbatementString);
    // 记录datetime
    MYSQL_SYNTHESIZE(string, recordedDatetime, RecordedDatetime);
    // 记录人标识
    MYSQL_SYNTHESIZE(string, recorderId, RecorderId);
    // stagesummary文本
    MYSQL_SYNTHESIZE(string, stageSummaryText, StageSummaryText);
    // 临床证据
    MYSQL_SYNTHESIZE(string, evidence, Evidence);
    // 医保编号
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    // 定义标识
    MYSQL_SYNTHESIZE(string, definitionId, DefinitionId);
    // tcm标志位
    MYSQL_SYNTHESIZE(int, tcmFlag, TcmFlag);
    // 诊断记录聚合根
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);

public:
    ClnConditionDO() : BaseDO("cln_condition")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("created_at", "s", createdAt);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
        MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("verification_status_enum", "s", verificationStatusEnum);
        MYSQL_ADD_FIELD("clinical_status_enum", "s", clinicalStatusEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("severity_enum", "s", severityEnum);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("body_site_json", "s", bodySiteJson);
        MYSQL_ADD_FIELD("body_structure_id", "s", bodyStructureId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("description", "s", description);
        MYSQL_ADD_FIELD("onset_datetime", "s", onsetDatetime);
        MYSQL_ADD_FIELD("onset_age", "i", onsetAge);
        MYSQL_ADD_FIELD("onset_text", "s", onsetText);
        MYSQL_ADD_FIELD("abatement_datetime", "s", abatementDatetime);
        MYSQL_ADD_FIELD("abatement_age", "i", abatementAge);
        MYSQL_ADD_FIELD("abatement_string", "s", abatementString);
        MYSQL_ADD_FIELD("recorded_datetime", "s", recordedDatetime);
        MYSQL_ADD_FIELD("recorder_id", "s", recorderId);
        MYSQL_ADD_FIELD("stage_summary_text", "s", stageSummaryText);
        MYSQL_ADD_FIELD("evidence", "s", evidence);
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        MYSQL_ADD_FIELD("definition_id", "s", definitionId);
        MYSQL_ADD_FIELD("tcm_flag", "i", tcmFlag);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
    }
};

// 给 ClnConditionDO 智能指针设定一个别名方便使用
typedef std::shared_ptr<ClnConditionDO> PtrClnConditionDO;
#endif // !_CLN_CONDITION_DO_H_