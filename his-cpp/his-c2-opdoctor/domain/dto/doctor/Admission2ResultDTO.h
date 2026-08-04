#pragma once
#ifndef __ADMISSION2RESULTDTO_H_
#define __ADMISSION2RESULTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Admission2ResultDTO :public oatpp::DTO
{
    DTO_INIT(Admission2ResultDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, applicationId, ZH_WORDS_GETTER("cyclectrl2.admission.applicationId"));
    API_DTO_FIELD_DEFAULT(String, applicationNo, ZH_WORDS_GETTER("cyclectrl2.admission.applicationNo"));
    API_DTO_FIELD_DEFAULT(String, status, ZH_WORDS_GETTER("cyclectrl2.admission.status"));
    API_DTO_FIELD_DEFAULT(String, statusName, ZH_WORDS_GETTER("cyclectrl2.admission.statusName"));
    API_DTO_FIELD_DEFAULT(String, createdAt, ZH_WORDS_GETTER("cyclectrl2.admission.createdAt"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
