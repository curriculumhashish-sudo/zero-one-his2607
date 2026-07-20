#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _SAMPLE_FULL_SELECT_JSONVO_H_
#define _SAMPLE_FULL_SELECT_JSONVO_H_

#include "../../GlobalInclude.h"
#include "../dto/SampleFullSelectDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 样本完整数据选择分页响应VO
 * 接口：/sample/full-select-page
 * 页面：携带完整样本数据、支持直接回填修改的样本选择弹窗
 */
class SampleFullSelectJsonVO : public JsonVO<SampleFullSelectDTO::Wrapper>
{
	DTO_INIT(SampleFullSelectJsonVO, JsonVO<SampleFullSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_FULL_SELECT_JSONVO_H_