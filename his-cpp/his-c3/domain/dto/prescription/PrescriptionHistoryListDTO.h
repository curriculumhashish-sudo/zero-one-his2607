#pragma once

#ifndef _PRESCRIPTION_HISTORY_LIST_DTO_
#define _PRESCRIPTION_HISTORY_LIST_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionHistoryDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个查询历史处方的List数据传输模型
* 再封装一层的原因是为了增加historyRows字段
*/
class PrescriptionHistoryListDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionHistoryListDTO, DTO);
	// 封装一层historyRows
	API_DTO_FIELD_DEFAULT(List<PrescriptionHistoryDTO::Wrapper>, historyRows, ZH_WORDS_GETTER("prescription.field.historyRows"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_HISTORY_LIST_DTO_