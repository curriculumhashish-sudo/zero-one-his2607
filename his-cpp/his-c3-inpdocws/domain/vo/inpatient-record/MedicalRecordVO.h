#pragma once
#ifndef _MEDICALRECORDVO_H_
#define _MEDICALRECORDVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/inpatient-record/MedicalRecordDTO.h"

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
class ByTitlePageJsonVO : public JsonVO<MedicalRecordBriefPageDTO::Wrapper> {
	DTO_INIT(ByTitlePageJsonVO, JsonVO<MedicalRecordBriefPageDTO::Wrapper>);
};

class ByIdJsonVO : public JsonVO<MedicalRecordDetailDTO::Wrapper> {
	DTO_INIT(ByIdJsonVO, JsonVO<MedicalRecordDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_