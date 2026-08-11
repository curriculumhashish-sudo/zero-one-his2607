#ifndef __CHINAPRES2REMOVERESPWRAPPERDTO_H_
#define __CHINAPRES2REMOVERESPWRAPPERDTO_H_

#include "../../GlobalInclude.h"
#include "Chinapres2RemoveRespDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class Chinapres2RemoveRespWrapperDTO : public oatpp::DTO {
    DTO_INIT(Chinapres2RemoveRespWrapperDTO, DTO);
    API_DTO_FIELD(Int32, code, "code", false, 0);
    API_DTO_FIELD(String, message, "message", false, "");
    API_DTO_FIELD(Object<Chinapres2RemoveRespDTO>, data, "data", false, nullptr);
};

#include OATPP_CODEGEN_END(DTO)
#endif