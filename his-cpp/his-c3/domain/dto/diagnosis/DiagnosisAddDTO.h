#pragma once
#ifndef _DIAGNOSIS_ADD_DTO_H_
#define _DIAGNOSIS_ADD_DTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断添加DTO类
   */
class DiagnosisAddDTO : public oatpp::DTO
{
	DTO_INIT(DiagnosisAddDTO, DTO);
	//患者id
	API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("diagnosis.field.tenantId"), true);
	//就诊id
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("diagnosis.field.encounterId"), true);
	//是否主诊断
	API_DTO_FIELD_REQUIRE(Int32, primaryFlag, ZH_WORDS_GETTER("diagnosis.field.primaryFlag"), false);
	//是否设为常用
	API_DTO_FIELD_REQUIRE(Int32, setAlways, ZH_WORDS_GETTER("diagnosis.field.setAlways"), false);
	//排序
	API_DTO_FIELD_REQUIRE(Int32, rank, ZH_WORDS_GETTER("diagnosis.field.rank"), false);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("diagnosis.field.description"), true);
	//病情编码(如CDF00000003)
	API_DTO_FIELD_REQUIRE(Int32, conditionCode, ZH_WORDS_GETTER("diagnosis.field.conditionCode"), false);
	//是否中医
	API_DTO_FIELD_REQUIRE(Int32, tcmFlag, ZH_WORDS_GETTER("diagnosis.field.tcmFlag"), false);
	//疾病id
	//API_DTO_FIELD_REQUIRE(String, disease_id, ZH_WORDS_GETTER("diagnosis.field.disease_def_id"), true);
	//证候id
	//API_DTO_FIELD_REQUIRE(String, syndrome_id, ZH_WORDS_GETTER("diagnosis.field.syndrome_def_id"), true);
	//病情/诊断条件id
	//API_DTO_FIELD_REQUIRE(String, conditionId, ZH_WORDS_GETTER("diagnosis.field.conditionId"), true);
	//患者id
	API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("diagnosis.field.patientId"), true);
	
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	std::string validate()
	{
		if (!tenantId || tenantId->empty())
			return "tenantId invalidate.";
		if (!encounterId || encounterId->empty())
			return "encounterId invalidate.";
		if (!description || description->empty())
			return "description invalidate.";
		if(!patientId || patientId->empty())
			return "patientId invalidate.";
		return "";
	}
	
};





#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSIS_ADD_DTO_H_
