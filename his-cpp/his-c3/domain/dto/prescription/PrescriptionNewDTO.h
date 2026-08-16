#pragma once

#ifndef _PRESCRIPTION_NEW_DTO_
#define _PRESCRIPTION_NEW_DTO_

#include "../../GlobalInclude.h"
#include "PrescriptionNewPrDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 定义一个新增处方的数据传输模型
*/
class PrescriptionNewDTO : public oatpp::DTO
{
	// 定义初始化
	DTO_INIT(PrescriptionNewDTO, DTO);
	// 挂号id
	API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("prescription.field.encounterId"));
	// 病人id
	API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("prescription.field.patientId"));
	// 租户id
	API_DTO_FIELD_DEFAULT(String, tenantId, ZH_WORDS_GETTER("prescription.field.tenantId"));
	// 封装一层prescriptions
	API_DTO_FIELD_DEFAULT(List<PrescriptionNewPrDTO::Wrapper>, prescriptions, ZH_WORDS_GETTER("prescription.field.prescriptions"));

	std::string validate()
	{
		if (encounterId.getValue("").empty())
			return "encounter id invalidate.";
		if (patientId.getValue("").empty())
			return "patient id invalidate.";
		if (tenantId.getValue("").empty())
			return "tenant id invalidate.";
		// Check every prescription's prescriptionGroupKey is not empty
		if (!prescriptions || prescriptions->empty())
			return "prescriptions invalidate.";
		for (const auto& presc : *prescriptions)
		{
			if (!presc || presc->prescriptionGroupKey.getValue("").empty())
				return "prescription group key invalidate.";
			if (!presc->lines || presc->lines->empty())
				return "lines empty";
			else {
				for (const auto& one : *presc->lines) {
					if (!one || !one->medicationDefinitionId || one->medicationDefinitionId->empty())
						return "lines has empty element";
				}
			}
		}
		return "";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_DTO_