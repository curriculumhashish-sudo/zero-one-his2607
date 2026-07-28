#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/22 10:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _DIAGNOSIS_SELECT_PAGE_DTO_H_
#define _DIAGNOSIS_SELECT_PAGE_DTO_H_

#include "../../GlobalInclude.h"
#include "DiagnoseSelectDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

<<<<<<< HEAD:his-cpp/his-c6-basedata/domain/dto/Diagnose/DiagnoseSelectPageDTO.h
/**
 * Paged transfer object for selectable diagnoses.
 */
class DiagnoseSelectPageDTO : public PageDTO<DiagnoseSelectDTO::Wrapper>
=======
/** 可选诊疗分页结果。 */
class DiagnosisSelectPageDTO : public PageDTO<DiagnosisSelectDTO::Wrapper>
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b:his-cpp/his-c6-basedata/domain/dto/Diagnosis/DiagnosisSelectPageDTO.h
{
	DTO_INIT(DiagnoseSelectPageDTO, PageDTO<DiagnoseSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_SELECT_PAGE_DTO_H_
