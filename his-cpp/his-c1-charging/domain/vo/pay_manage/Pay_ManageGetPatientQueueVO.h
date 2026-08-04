#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

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

#include "../../GlobalInclude.h"
#include "../../dto/pay_manage/Pay_ManageGetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientQueuePageJsonVO : public JsonVO<Pay_ManageGetPatientQueuePageDTO::Wrapper> {
	DTO_INIT(Pay_ManageGetPatientQueuePageJsonVO, JsonVO<Pay_ManageGetPatientQueuePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
