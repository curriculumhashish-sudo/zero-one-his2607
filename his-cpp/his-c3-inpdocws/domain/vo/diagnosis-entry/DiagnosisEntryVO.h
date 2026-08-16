#pragma once
#ifndef _DIAGNOSISENTRYVO_H_
#define _DIAGNOSISENTRYVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/diagnosis-entry/DiagnosisEntryDTO.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
#include OATPP_CODEGEN_BEGIN(DTO)

///**
// * 示例显示JsonVO，用于响应给客户端的Json对象
// */
//class SampleJsonVO : public JsonVO<SampleDTO::Wrapper> {
//	DTO_INIT(SampleJsonVO, JsonVO<SampleDTO::Wrapper>);
//};

/**
 * 示例分页显示JsonVO，用于响应给客户端的Json对象
 */
class DiagnosisEntryPageJsonVO : public JsonVO<DiagnosisEntryBriefPageDTO::Wrapper> {
	DTO_INIT(DiagnosisEntryPageJsonVO, JsonVO<DiagnosisEntryBriefPageDTO::Wrapper>);
};

class DiagnosisEntryDetailPageJsonVO : public JsonVO<DiagnosisEntryDetailPageDTO::Wrapper> {
	DTO_INIT(DiagnosisEntryDetailPageJsonVO, JsonVO<DiagnosisEntryDetailPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_