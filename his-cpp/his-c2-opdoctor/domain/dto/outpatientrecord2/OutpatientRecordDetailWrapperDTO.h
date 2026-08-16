#ifndef __OUTPATIENTRECORDDETAILWRAPPERDTO_H_
#define __OUTPATIENTRECORDDETAILWRAPPERDTO_H_

#include "../../GlobalInclude.h"
#include "OutpatientRecordDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientRecordDetailWrapperDTO : public oatpp::DTO {
    DTO_INIT(OutpatientRecordDetailWrapperDTO, DTO);
    API_DTO_FIELD(Int32, code, "code", false, 0);
    API_DTO_FIELD(String, message, "message", false, "");
    API_DTO_FIELD(Object<OutpatientRecordDetailDTO>, data, "data", false, nullptr);
};

#include OATPP_CODEGEN_END(DTO)
#endif