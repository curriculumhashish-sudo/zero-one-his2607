#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/08/08 10:00:00

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
#ifndef _INPATIENTADMISSIONWARDVO_H_
#define _INPATIENTADMISSIONWARDVO_H_

#include "domain/GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Ward option view object for the inpatient admission form.
 * value is the ward (adm_location) primary key used as wardId when
 * submitting an admission application; label is the ward name;
 * departmentId is the owning inpatient department id which must be
 * consistent with the requested department id.
 */
class InpatientAdmissionWardVO : public oatpp::DTO
{
	DTO_INIT(InpatientAdmissionWardVO, DTO);

	// Ward primary key, submitted as wardId
	API_DTO_FIELD_DEFAULT(String, value, ZH_WORDS_GETTER("cyclectrl2.admission.wardId"));
	// Ward display name
	API_DTO_FIELD_DEFAULT(String, label, ZH_WORDS_GETTER("cyclectrl2.admission.wardName"));
	// Owning inpatient department id
	API_DTO_FIELD_DEFAULT(String, departmentId, ZH_WORDS_GETTER("cyclectrl2.admission.departmentId"));
};

/**
 * List response VO for the wards endpoint.
 */
class InpatientAdmissionWardListJsonVO : public ListJsonVO<InpatientAdmissionWardVO::Wrapper>
{
	DTO_INIT(InpatientAdmissionWardListJsonVO, ListJsonVO<InpatientAdmissionWardVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENTADMISSIONWARDVO_H_