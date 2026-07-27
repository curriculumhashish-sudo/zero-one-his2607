#pragma once
#ifndef _PATIENTFINISHDTO_H_
#define _PATIENTFINISHDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientFinishDTO : public oatpp::DTO
{
    DTO_INIT(PatientFinishDTO, DTO);

  
    // 更新后最新就诊状态
    API_DTO_FIELD_REQUIRE(String, newVisitStatus, ZH_WORDS_GETTER("finish.dto.newVisitStatus"), true);

    //唯一标识
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("finish.dto.encounterId"), false);

};


#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENTFINISHDTO_H_
