#pragma once
#ifndef __PRESCRIPTIONDTO_H_
#define __PRESCRIPTIONDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Prescription2DetailDTO :public oatpp::DTO
{
    DTO_INIT(Prescription2DetailDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, patientName, ZH_WORDS_GETTER("cyclectrl2.patient.patientName"));
    API_DTO_FIELD_DEFAULT(Int32, age, ZH_WORDS_GETTER("cyclectrl2.patient.age"));
    API_DTO_FIELD_DEFAULT(String, gender, ZH_WORDS_GETTER("cyclectrl2.patient.gender"));
    API_DTO_FIELD_DEFAULT(String, department, ZH_WORDS_GETTER("cyclectrl2.patient.department"));
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("cyclectrl2.patient.patientId"));

    API_DTO_FIELD_DEFAULT(String, rxId, ZH_WORDS_GETTER("cyclectrl2.prescription.rxId"));
    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("cyclectrl2.prescription.encounterId"));
    API_DTO_FIELD_DEFAULT(String, prescriptionNo, ZH_WORDS_GETTER("cyclectrl2.prescription.prescriptionNo"));
    API_DTO_FIELD_DEFAULT(String, costType, ZH_WORDS_GETTER("cyclectrl2.prescription.costType"));
    API_DTO_FIELD_DEFAULT(String, date, ZH_WORDS_GETTER("cyclectrl2.prescription.date"));
    API_DTO_FIELD_DEFAULT(String, issuedTime, ZH_WORDS_GETTER("cyclectrl2.prescription.issuedTime"));
    API_DTO_FIELD_DEFAULT(String, status, ZH_WORDS_GETTER("cyclectrl2.prescription.status"));
    API_DTO_FIELD_DEFAULT(String, statusName, ZH_WORDS_GETTER("cyclectrl2.prescription.statusName"));
    API_DTO_FIELD_DEFAULT(String, rxType, ZH_WORDS_GETTER("cyclectrl2.prescription.rxType"));
    API_DTO_FIELD_DEFAULT(String, doctor, ZH_WORDS_GETTER("cyclectrl2.prescription.doctor"));
    API_DTO_FIELD_DEFAULT(String, diagnosis, ZH_WORDS_GETTER("cyclectrl2.prescription.diagnosis"));
    API_DTO_FIELD_DEFAULT(Float64, totalPrice, ZH_WORDS_GETTER("cyclectrl2.prescription.totalPrice"));
    API_DTO_FIELD_DEFAULT(String, pharmacist, ZH_WORDS_GETTER("cyclectrl2.prescription.pharmacist"));
    API_DTO_FIELD_DEFAULT(String, dispenser, ZH_WORDS_GETTER("cyclectrl2.prescription.dispenser"));
    API_DTO_FIELD_DEFAULT(String, priceChecker, ZH_WORDS_GETTER("cyclectrl2.prescription.priceChecker"));
    API_DTO_FIELD_DEFAULT(String, items, ZH_WORDS_GETTER("cyclectrl2.prescription.items"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
