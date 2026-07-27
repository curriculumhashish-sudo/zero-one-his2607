#pragma once
#ifndef _GET_PATIENT_LIST_DO_
#define _GET_PATIENT_LIST_DO_
#include "../DoInclude.h"


class GetPatientListDO : public BaseDO
{
    
    MYSQL_SYNTHESIZE(long long, encounter_id, Encounter_id);
    MYSQL_SYNTHESIZE(string, inpatient_no, Inpatient_no);
    MYSQL_SYNTHESIZE(string, patient_name, Patient_name);
    MYSQL_SYNTHESIZE(int, gender, Gender);
    MYSQL_SYNTHESIZE(int, age, Age);
    MYSQL_SYNTHESIZE(string, dept_name, Dept_name);
    MYSQL_SYNTHESIZE(string, ward_name, Ward_name);
    MYSQL_SYNTHESIZE(string, bed_no, Bed_no);
    MYSQL_SYNTHESIZE(long long, admit_date, Admit_date);
    MYSQL_SYNTHESIZE(int, insurance_type, Insurance_type);
    MYSQL_SYNTHESIZE(int, encounter_status, Encounter_status);

public:
    GetPatientListDO() : BaseDO("PatientList")
    {
      
        MYSQL_ADD_FIELD_PK("encounter_id", "ll", encounter_id);
        MYSQL_ADD_FIELD("inpatient_no", "s", inpatient_no);
        MYSQL_ADD_FIELD("patient_name", "s", patient_name);
        MYSQL_ADD_FIELD("gender", "i", gender);
        MYSQL_ADD_FIELD("age", "i", age);
        MYSQL_ADD_FIELD("dept_name", "s", dept_name);
        MYSQL_ADD_FIELD("ward_name", "s", ward_name);
        MYSQL_ADD_FIELD("bed_no", "s", bed_no);
        MYSQL_ADD_FIELD("admit_date", "dt", admit_date);
        MYSQL_ADD_FIELD("insurance_type", "i", insurance_type);
        MYSQL_ADD_FIELD("encounter_status", "i", encounter_status);
    }
};


typedef std::shared_ptr<GetPatientListDO> PtrGetPatientListDO;

#endif