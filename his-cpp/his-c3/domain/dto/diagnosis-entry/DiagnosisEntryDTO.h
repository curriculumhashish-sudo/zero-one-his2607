#pragma once
#ifndef _DIAGNOSISENTRYDTO_H_
#define _DIAGNOSISENTRYDTO_H_

#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY

//获取获取诊断使用历史DTO类
class DiagnosisEntryBriefDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(DiagnosisEntryBriefDTO, DTO);
	////诊断主键
	//API_DTO_FIELD_REQUIRE(String, condition_id, ZH_WORDS_GETTER("diagnosis-entry.condition_id"), true);
	////诊断症状id
	//API_DTO_FIELD_REQUIRE(String, definition_id, ZH_WORDS_GETTER("diagnosis-entry.definition_id"), true);
	//诊断症状名称
	API_DTO_FIELD_REQUIRE(String, definition_name, ZH_WORDS_GETTER("diagnosis-entry.definition_name"), true);
};
class DiagnosisEntryBriefPageDTO : public PageDTO<DiagnosisEntryBriefDTO::Wrapper>
{
	DTO_INIT(DiagnosisEntryBriefPageDTO, PageDTO<DiagnosisEntryBriefDTO::Wrapper>);
};


//获取患者诊断记录（条件+分页）DTO类
class DiagnosisEntryDetailDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(DiagnosisEntryDetailDTO, DTO);
	//诊断主键
	API_DTO_FIELD_REQUIRE(String, condition_id, ZH_WORDS_GETTER("diagnosis-entry.condition_id"), true);
	////诊断症状id
	//API_DTO_FIELD_REQUIRE(String, definition_id, ZH_WORDS_GETTER("diagnosis-entry.definition_id"), true);
	//诊断症状编码
	API_DTO_FIELD_REQUIRE(String, definition_code, ZH_WORDS_GETTER("diagnosis-entry.definition_code"), true);
	//诊断描述
	API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("diagnosis-entry.description"), true);
	////医学类型编码
	//API_DTO_FIELD_REQUIRE(String, kind_code, ZH_WORDS_GETTER("diagnosis-entry.kind_code"), true);
	//医学类型名称
	API_DTO_FIELD_REQUIRE(String, kind_name, ZH_WORDS_GETTER("diagnosis-entry.kind_text"), true);
	////诊断类型编码
	//API_DTO_FIELD_REQUIRE(String, type_code, ZH_WORDS_GETTER("diagnosis-entry.type_code"), true);
	//诊断类型名称
	API_DTO_FIELD_REQUIRE(String, type_name, ZH_WORDS_GETTER("diagnosis-entry.type_name"), true);
	//主诊标注
	API_DTO_FIELD_REQUIRE(String, main_flag, ZH_WORDS_GETTER("diagnosis-entry.main_flag"), true);
	//主诊标注
	API_DTO_FIELD_REQUIRE(String, adm_encounter_diagnosis_id, ZH_WORDS_GETTER("diagnosis-entry.adm_encounter_diagnosis_id"), true);
};
class DiagnosisEntryDetailPageDTO : public PageDTO<DiagnosisEntryDetailDTO::Wrapper>
{
	DTO_INIT(DiagnosisEntryDetailPageDTO, PageDTO<DiagnosisEntryDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
