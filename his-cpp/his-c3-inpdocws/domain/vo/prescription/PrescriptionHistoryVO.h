#pragma once

#ifndef _PRESCRIPTION_HISTORY_VO_
#define _PRESCRIPTION_HISTORY_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrescriptionHistoryListDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 历史处方显示JsonVO，用于响应给客户端的Json对象
*/
class PrescriptionHistoryListJsonVO : public JsonVO<PrescriptionHistoryListDTO::Wrapper>
{
	DTO_INIT(PrescriptionHistoryListJsonVO, JsonVO<PrescriptionHistoryListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_HISTORY_VO_
