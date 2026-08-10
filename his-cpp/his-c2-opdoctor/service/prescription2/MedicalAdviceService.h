#pragma once

#ifndef _MEDICAL_ADVICESERVICE_H_
#define _MEDICAL_ADVICESERVICE_H_

#include "domain/dto/prescription2/PrintApplicationFormDTO.h"
#include "domain/dto/prescription2/SignMedicalAdviceDTO.h"
#include "domain/dto/prescription2/WithdrawMedicalAdviceDTO.h"
#include <list>

// 医嘱业务服务
class MedicalAdviceService
{
public:
    // 将草稿状态医嘱记录变更为有效状态
    std::list<std::string> signMedicalAdvice(
        const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // 将有效医嘱记录变更为已取消状态
    std::list<std::string> withdrawMedicalAdvice(
        const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload);
    // 增加打印次数并返回服务申请单标识
    std::string printApplicationForm(
        const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload);

private:
    // 移除空标识和重复标识，同时保留请求顺序
    std::list<std::string> normalizeAdviceIds(
        const oatpp::List<oatpp::String>& adviceIds) const;
};

#endif // !_MEDICAL_ADVICESERVICE_H_
