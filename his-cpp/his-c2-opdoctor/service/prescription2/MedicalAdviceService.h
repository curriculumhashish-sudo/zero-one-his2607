#pragma once

#ifndef _MEDICAL_ADVICESERVICE_H_
#define _MEDICAL_ADVICESERVICE_H_

#include "domain/dto/prescription2/PrintApplicationFormDTO.h"
#include "domain/dto/prescription2/SignMedicalAdviceDTO.h"
#include "domain/dto/prescription2/WithdrawMedicalAdviceDTO.h"
#include <list>

// Medical advice business service.
class MedicalAdviceService
{
public:
    // Change medical advice records from draft to signed status.
    std::list<std::string> signMedicalAdvice(
        const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // Change signed medical advice records to withdrawn status.
    std::list<std::string> withdrawMedicalAdvice(
        const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // Record one service request print operation.
    std::string printApplicationForm(
        const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload);

private:
    // Remove empty and duplicate identifiers while preserving request order.
    std::list<std::string> normalizeAdviceIds(
        const oatpp::List<oatpp::String>& adviceIds) const;
};

#endif // !_MEDICAL_ADVICESERVICE_H_
