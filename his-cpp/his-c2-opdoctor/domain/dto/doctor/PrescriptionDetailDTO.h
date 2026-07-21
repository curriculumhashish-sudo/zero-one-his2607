#pragma once
#ifndef __PRESCRIPTIONDTO_H_
#define __PRESCRIPTIONDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrescriptionDetailDTO :public oatpp::DTO
{
    DTO_INIT(PrescriptionDetailDTO, DTO);

    //患者信息
    API_DTO_FIELD_DEFAULT(String, patientName, ZH_WORDS_GETTER("patient.detail.patientName"));     // 姓名
    API_DTO_FIELD_DEFAULT(Int32, age, ZH_WORDS_GETTER("patient.detail.age"));                       // 年龄
    API_DTO_FIELD_DEFAULT(String, gender, ZH_WORDS_GETTER("patient.detail.gender"));                // 性别
    API_DTO_FIELD_DEFAULT(String, department, ZH_WORDS_GETTER("patient.detail.department"));        // 科室
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("patient.detail.patientId"));          // 门诊号

    //处方信息
    API_DTO_FIELD_DEFAULT(String, prescriptionNo, ZH_WORDS_GETTER("Prescription.detail.prescriptionNo")); // 处方号
    API_DTO_FIELD_DEFAULT(String, costType, ZH_WORDS_GETTER("Prescription.detail.costType"));             // 费用性质
    API_DTO_FIELD_DEFAULT(String, date, ZH_WORDS_GETTER("Prescription.detail.date"));                     // 日期
    API_DTO_FIELD_DEFAULT(String, doctor, ZH_WORDS_GETTER("Prescription.detail.doctor"));                 // 开单医生
    API_DTO_FIELD_DEFAULT(String, diagnosis, ZH_WORDS_GETTER("Prescription.detail.diagnosis"));           // 诊断
    API_DTO_FIELD_DEFAULT(Float64, totalPrice, ZH_WORDS_GETTER("Prescription.detail.totalPrice"));        // 合计金额
    API_DTO_FIELD_DEFAULT(String, pharmacist, ZH_WORDS_GETTER("Prescription.detail.pharmacist"));         // 调配
    API_DTO_FIELD_DEFAULT(String, dispenser, ZH_WORDS_GETTER("Prescription.detail.dispenser"));           // 发药
    API_DTO_FIELD_DEFAULT(String, priceChecker, ZH_WORDS_GETTER("Prescription.detail.priceChecker"));     // 划价
    API_DTO_FIELD_DEFAULT(String, items, ZH_WORDS_GETTER("Prescription.detail.items")); // Rp

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUERY_H_

