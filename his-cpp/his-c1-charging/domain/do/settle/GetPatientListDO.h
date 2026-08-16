#pragma once
#ifndef _GET_PATIENT_LIST_DO_
#define _GET_PATIENT_LIST_DO_
#include "../DoInclude.h"

class GetPatientListDO : public BaseDO
{
    // 就诊ID
    MYSQL_SYNTHESIZE(string, id, Id);
    // 患者主键 (对应 pat.id)
    MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
    // 账户主键 (对应 adm_account.id)
    MYSQL_SYNTHESIZE(string, account_id, Account_id);
    // 住院号 (对应 enc.bus_no)
    MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
    // 患者姓名 (对应 pat.name)
    MYSQL_SYNTHESIZE(string, name, Name);
    // 性别 (对应 pat.gender_enum)
    MYSQL_SYNTHESIZE(int, gender_enum, Gender_enum);
    // 年龄 (SQL中计算得出)
    MYSQL_SYNTHESIZE(int, age, Age);
    // 科室名称 (对应 loc_dept.name)
    MYSQL_SYNTHESIZE(string, org_name, Org_name);
    // 病区名称 (对应 loc_ward.name)
    MYSQL_SYNTHESIZE(string, ward_name, Ward_name);
    // 床号 (对应 loc_bed.name)
    MYSQL_SYNTHESIZE(string, bed_no, Bed_no);
    //【新增】原始入院时间，用于 ORDER BY 语法兼容
    MYSQL_SYNTHESIZE(string, start_time, Start_time);
    // 入院日期字符串（格式化后展示）
    MYSQL_SYNTHESIZE(string, admit_date, Admit_date);
    // 医保类型 (对应 enc.yb_class_enum)
    MYSQL_SYNTHESIZE(int, insurance_type, Insurance_type);
    // 医生姓名 (对应 prac.name)
    MYSQL_SYNTHESIZE(string, doctor_name, Doctor_name);

public:
    // 多表JOIN查询DO，不绑定单表名，构造参数填空字符串
    GetPatientListDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("patient_id", "s", patient_id);
        MYSQL_ADD_FIELD("account_id", "s", account_id);
        MYSQL_ADD_FIELD("bus_no", "s", bus_no);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("gender_enum", "i", gender_enum);
        MYSQL_ADD_FIELD("age", "i", age);
        MYSQL_ADD_FIELD("org_name", "s", org_name);
        MYSQL_ADD_FIELD("ward_name", "s", ward_name);
        MYSQL_ADD_FIELD("bed_no", "s", bed_no);
        // ✅新增字段映射，顺序必须和SQL SELECT保持一致
        MYSQL_ADD_FIELD("start_time", "s", start_time);
        MYSQL_ADD_FIELD("admit_date", "s", admit_date);
        MYSQL_ADD_FIELD("insurance_type", "i", insurance_type);
        MYSQL_ADD_FIELD("doctor_name", "s", doctor_name);
    }
};

typedef std::shared_ptr<GetPatientListDO> PtrGetPatientListDO;

#endif // !_GET_PATIENT_LIST_DO_

