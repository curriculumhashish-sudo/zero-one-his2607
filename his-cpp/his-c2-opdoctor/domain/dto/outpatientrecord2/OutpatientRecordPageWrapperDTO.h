#ifndef __OUTPATIENTRECORDPAGEWRAPPERDTO_H_
#define __OUTPATIENTRECORDPAGEWRAPPERDTO_H_

#include "../../GlobalInclude.h"
#include "../../vo/outpatientrecord2/OutpatientRecordVO.h"  // OutpatientRecordPageJsonVO
#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientRecordPageWrapperDTO : public oatpp::DTO {
    DTO_INIT(OutpatientRecordPageWrapperDTO, DTO);
    API_DTO_FIELD(Int32, code, "code", false, 0);
    API_DTO_FIELD(String, message, "message", false, "");
    API_DTO_FIELD(Object<OutpatientRecordPageDTO>, data, "data", false, nullptr);
};

#include OATPP_CODEGEN_END(DTO)
#endif