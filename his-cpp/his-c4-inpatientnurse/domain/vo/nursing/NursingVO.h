#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/19 10:00:00

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
#ifndef _NURSING_VO_
    #define _NURSING_VO_
    #include "../../GlobalInclude.h"
    #include "../../dto/nursing/NursingDetailDTO.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

/**
 */
class NursingJsonVO : public JsonVO<NursingDetailDTO::Wrapper>
{
    DTO_INIT(NursingJsonVO, JsonVO<NursingDetailDTO::Wrapper>);
};

/**
 */
class NursingPageJsonVO : public JsonVO<NursingPageDTO::Wrapper>
{
    DTO_INIT(NursingPageJsonVO, JsonVO<NursingPageDTO::Wrapper>);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_NURSING_VO_
