#pragma once
#ifndef _PATIENT_PAGE_ITEM_DTO_H_
#define _PATIENT_PAGE_ITEM_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/** One row in the patient management page. */
class PatientPageItemDTO : public oatpp::DTO
{
    DTO_INIT(PatientPageItemDTO, DTO);

    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.id");
    }

    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.busNo");
    }

    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.name");
    }

    DTO_FIELD(String, idCard);
    DTO_FIELD_INFO(idCard) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.idCard");
    }

    DTO_FIELD(Int32, gender);
    DTO_FIELD_INFO(gender) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.gender");
    }

    DTO_FIELD(Int32, age);
    DTO_FIELD_INFO(age) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.age");
    }

    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("pat.dto.field.phone");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PATIENT_PAGE_ITEM_DTO_H_
