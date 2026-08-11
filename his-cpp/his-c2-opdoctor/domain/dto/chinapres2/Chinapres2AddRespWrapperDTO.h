#ifndef __CHINAPRES2ADDRESPWRAPPERDTO_H_
#define __CHINAPRES2ADDRESPWRAPPERDTO_H_

#include "../../GlobalInclude.h"
#include "Chinapres2AddRespDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Chinapres2AddRespWrapperDTO : public oatpp::DTO {
    DTO_INIT(Chinapres2AddRespWrapperDTO, DTO);
    API_DTO_FIELD(Int32, code, "code", false, 0);
    API_DTO_FIELD(String, message, "message", false, "");
    API_DTO_FIELD(Object<Chinapres2AddRespDTO>, data, "data", false, nullptr);
};

#include OATPP_CODEGEN_END(DTO)
#endif