#pragma once
#ifndef _MEDICAL_ADVICESERVICE_H_
#define _MEDICAL_ADVICESERVICE_H_

#include "domain/dto/prescription2/PrintApplicationFormDTO.h"
#include "domain/dto/prescription2/SignMedicalAdviceDTO.h"
#include "domain/dto/prescription2/WithdrawMedicalAdviceDTO.h"
#include <list>

// 提供医嘱及打印记录的事务性操作。
class MedicalAdviceService
{
public:
    // 将处于 DRAFT 状态的医嘱记录变更为 COMPLETED。
    std::list<std::string> signMedicalAdvice(
        const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // 将处于 COMPLETED 状态的医嘱记录恢复为 DRAFT。
    std::list<std::string> withdrawMedicalAdvice(
        const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // 增加服务申请单的打印次数。
    std::string printApplicationForm(
        const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload);

private:
    // 在保留原顺序的前提下移除空标识和重复标识。
    std::list<std::string> normalizeMedicalAdviceIds(
        const oatpp::List<oatpp::String>& medicalAdviceIds) const;
};

#endif // !_MEDICAL_ADVICESERVICE_H_
