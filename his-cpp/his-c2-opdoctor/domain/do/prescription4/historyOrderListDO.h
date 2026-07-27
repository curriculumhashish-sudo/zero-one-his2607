#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_LIST_DO_
#define _HISTORY_ORDER_LIST_DO_
#include "../DoInclude.h"

/**
 * 历史医嘱就诊记录列表 DO
 * 对应左侧就诊卡片展示字段，来自多表关联查询
 */
class HistoryOrderListDO : public BaseDO
{
    // 就诊记录ID
    MYSQL_SYNTHESIZE(string, visitId, VisitId);
    // 就诊时间
    MYSQL_SYNTHESIZE(string, visitTime, VisitTime);
    // 就诊类型
    MYSQL_SYNTHESIZE(string, visitType, VisitType);
    // 主诊断
    MYSQL_SYNTHESIZE(string, mainDiagnosis, MainDiagnosis);
    // 接诊医生姓名
    MYSQL_SYNTHESIZE(string, doctorName, DoctorName);
    // 药品医嘱数量
    MYSQL_SYNTHESIZE(int, drugCount, DrugCount);
    // 检验检查医嘱数量
    MYSQL_SYNTHESIZE(int, examCount, ExamCount);
    // 耗材医嘱数量
    MYSQL_SYNTHESIZE(int, materialCount, MaterialCount);

public:
    HistoryOrderListDO() : BaseDO("adm_encounter")   // 主表为就诊表
    {
        MYSQL_ADD_FIELD_PK("id", "s", visitId);                // 使用就诊ID作为主键
        MYSQL_ADD_FIELD("visit_time", "s", visitTime);
        MYSQL_ADD_FIELD("visit_type", "s", visitType);
        MYSQL_ADD_FIELD("main_diagnosis", "s", mainDiagnosis);
        MYSQL_ADD_FIELD("doctor_name", "s", doctorName);
        MYSQL_ADD_FIELD("drug_count", "i", drugCount);
        MYSQL_ADD_FIELD("exam_count", "i", examCount);
        MYSQL_ADD_FIELD("material_count", "i", materialCount);
    }
};

typedef std::shared_ptr<HistoryOrderListDO> PtrHistoryOrderListDO;

#endif // !_HISTORY_ORDER_LIST_DO_