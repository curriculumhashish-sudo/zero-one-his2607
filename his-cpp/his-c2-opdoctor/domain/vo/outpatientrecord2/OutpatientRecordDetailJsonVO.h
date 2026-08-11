#ifndef __OUTPATIENTRECORDDETAILJSONVO_H_
#define __OUTPATIENTRECORDDETAILJSONVO_H_

#include "../../dto/outpatientrecord2/OutpatientRecordDetailWrapperDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientRecordDetailJsonVO : public OutpatientRecordDetailWrapperDTO {
    DTO_INIT(OutpatientRecordDetailJsonVO, OutpatientRecordDetailWrapperDTO);
};

#include OATPP_CODEGEN_END(DTO)
#endif