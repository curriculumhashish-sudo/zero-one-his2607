#pragma once
#ifndef _GET_PATIENT_LIST_DO_
#define _GET_PATIENT_LIST_DO_
#include "../DoInclude.h"


class GetPatientListDO : public BaseDO
{
    // 就诊ID
    MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
    // 住院号
    MYSQL_SYNTHESIZE(string, inpatient_no, Inpatient_no);
    // 患者姓名
    MYSQL_SYNTHESIZE(string, patient_name, Patient_name);
    // 性别
    MYSQL_SYNTHESIZE(int, gender, Gender);
    // 年龄
    MYSQL_SYNTHESIZE(int, age, Age);
    // 科室名称
    MYSQL_SYNTHESIZE(string, dept_name, Dept_name);
    // 病区名称
    MYSQL_SYNTHESIZE(string, ward_name, Ward_name);
    // 床号
    MYSQL_SYNTHESIZE(string, bed_no, Bed_no);
    // 入院时间戳
    MYSQL_SYNTHESIZE(string, admit_date, Admit_date);
    // 医保类型
    MYSQL_SYNTHESIZE(int, insurance_type, Insurance_type);
    // 医生姓名
    MYSQL_SYNTHESIZE(string, doctor_name, Doctor_name);

public:
    // 多表JOIN查询DO，不绑定单表名，构造参数填空字符串
    GetPatientListDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounter_id);
        MYSQL_ADD_FIELD("inpatient_no", "s", inpatient_no);
        MYSQL_ADD_FIELD("patient_name", "s", patient_name);
        MYSQL_ADD_FIELD("gender", "i", gender);
        MYSQL_ADD_FIELD("age", "i", age);
        MYSQL_ADD_FIELD("dept_name", "s", dept_name);
        MYSQL_ADD_FIELD("ward_name", "s", ward_name);
        MYSQL_ADD_FIELD("bed_no", "s", bed_no);
        MYSQL_ADD_FIELD("admit_date", "s", admit_date);
        MYSQL_ADD_FIELD("insurance_type", "i", insurance_type);
        MYSQL_ADD_FIELD("doctor_name", "s", doctor_name);
    }
};

typedef std::shared_ptr<GetPatientListDO> PtrGetPatientListDO;

#endif // !_GET_PATIENT_LIST_DO_
