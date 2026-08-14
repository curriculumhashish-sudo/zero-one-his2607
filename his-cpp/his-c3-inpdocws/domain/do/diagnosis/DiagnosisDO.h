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
#ifndef _DIAGNOSIS_DO_H_
#define _DIAGNOSIS_DO_H_
#include "../DoInclude.h"

/**
 * 管理域 就诊诊断 数据库实体类
 */
class DiagnosisDO : public BaseDO
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
    // 就诊诊断关联子实体
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    // 病情/诊断条件主键
    MYSQL_SYNTHESIZE(string, conditionId, ConditionId);
    // 是否主诊断
    MYSQL_SYNTHESIZE(int, primaryFlag, PrimaryFlag);
    // 排序序号
    MYSQL_SYNTHESIZE(int, rank, Rank);
    // 诊断描述
    MYSQL_SYNTHESIZE(string, description, Description);
    // 诊断类型码
    MYSQL_SYNTHESIZE(string, diagTypeCode, DiagTypeCode);
    // 诊断编码
    MYSQL_SYNTHESIZE(int, diagCode, DiagCode);
    // 病情编码
    MYSQL_SYNTHESIZE(int, conditionCode, ConditionCode);
    // 医保或诊疗类别
    MYSQL_SYNTHESIZE(string, medType, MedType);
    // 是否最高诊断
    MYSQL_SYNTHESIZE(int, highestDiagFlag, HighestDiagFlag);
    // 中医证候组
    MYSQL_SYNTHESIZE(string, tcmSyndromeGroup, TcmSyndromeGroup);
    // 是否中医诊断
    MYSQL_SYNTHESIZE(int, tcmFlag, TcmFlag);

public:
    DiagnosisDO() : BaseDO("adm_encounter_diagnosis")
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
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("condition_id", "s", conditionId);
        MYSQL_ADD_FIELD("primary_flag", "i", primaryFlag);
        MYSQL_ADD_FIELD("`rank`", "i", rank);
        MYSQL_ADD_FIELD("description", "s", description);
        MYSQL_ADD_FIELD("diag_type_code", "s", diagTypeCode);
        MYSQL_ADD_FIELD("diag_code", "i", diagCode);
        MYSQL_ADD_FIELD("condition_code", "i", conditionCode);
        MYSQL_ADD_FIELD("med_type", "s", medType);
        MYSQL_ADD_FIELD("highest_diag_flag", "i", highestDiagFlag);
        MYSQL_ADD_FIELD("tcm_syndrome_group", "s", tcmSyndromeGroup);
        MYSQL_ADD_FIELD("tcm_flag", "i", tcmFlag);
    }
};

// 给 DiagnosisDO 智能指针设定一个别名方便使用
typedef std::shared_ptr<DiagnosisDO> PtrDiagnosisDO;
#endif // !_DIAGNOSIS_DO_H_