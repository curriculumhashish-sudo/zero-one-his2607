#pragma once

#ifndef _PRESCRIPTIONRECALLDTO_H_
#define _PRESCRIPTIONRECALLDTO_H_
#include "../../GlobalInclude.h"
// #include "PrescriptionItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 撤回处方DTO
 */
class PrescriptionRecallDTO : public oatpp::DTO
{
	DTO_INIT(PrescriptionRecallDTO, DTO);

	// 处方ID
	API_DTO_FIELD_REQUIRE(
		String,
		prescriptionId,
		ZH_WORDS_GETTER("prescription.field.prescriptionId"),
		true);
};
#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTIONRecallDTO_H_#pragma once
