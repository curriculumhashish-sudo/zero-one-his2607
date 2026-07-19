#pragma once

#ifndef _QUIT_NUMBER_DTO_
#define _QUIT_NUMBER_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 确认退号DTO（接收前端传参）
 */
class QuitNumberDTO : public oatpp::DTO {
    DTO_INIT(QuitNumberDTO, DTO);

    // 就诊号
    DTO_FIELD(String, visitNumber);
    DTO_FIELD_INFO(visitNumber) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.visitNumber");
        info->required = true;
    }

    // 患者姓名
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.name");
        info->required = true;
    }

    // 联系电话
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.phone");
        info->required = true;
    }

    // 挂号科室
    DTO_FIELD(String, department);
    DTO_FIELD_INFO(department) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.department");
        info->required = true;
    }

    // 接诊医生
    DTO_FIELD(String, doctor);
    DTO_FIELD_INFO(doctor) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.doctor");
        info->required = true;
    }

    // 挂号类型
    DTO_FIELD(String, registrationType);
    DTO_FIELD_INFO(registrationType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.registrationType");
        info->required = true;
    }

    // 挂号时间
    DTO_FIELD(String, registrationTime);
    DTO_FIELD_INFO(registrationTime) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.registrationTime");
        info->required = true;
    }

    // 应退金额
    DTO_FIELD(Float64, refundAmount);
    DTO_FIELD_INFO(refundAmount) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.refundAmount");
        info->required = true;
    }

    // 当前状态
    DTO_FIELD(String, status);
    DTO_FIELD_INFO(status) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.status");
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_QUIT_NUMBER_DTO_
