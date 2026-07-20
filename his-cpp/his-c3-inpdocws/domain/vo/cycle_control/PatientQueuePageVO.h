#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:34:14

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
#ifndef _PATIENT_QUEUE_PAGE_VO_H_
#define _PATIENT_QUEUE_PAGE_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/cycle_control/PatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者队列分页数据传输对象，包装PatientQueueDTO列表。
 */
class PatientQueuePageDTO : public PageDTO<PatientQueueDTO::Wrapper>
{
	DTO_INIT(PatientQueuePageDTO, PageDTO<PatientQueueDTO::Wrapper>);
public:
	PatientQueuePageDTO() {
		this->pageIndex = 1;
		this->pageSize = 10;
		this->total = 1;
		this->pages = 1;
		auto row = PatientQueueDTO::createShared();
		this->rows->push_back(row);
	}
};

/**
 * 患者队列分页响应JSON视图对象。
 */
class PatientQueuePageVO : public JsonVO<PatientQueuePageDTO::Wrapper> {
	DTO_INIT(PatientQueuePageVO, JsonVO<PatientQueuePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_QUEUE_PAGE_VO_H_
