/*
 Copyright Zero One Star. All rights reserved.

 停嘱结果 DTO

 @Author: nick1484065161
 @Date: 2026/07/20 00:00:00

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
#pragma once

// 防止头文件被重复包含
#ifndef _TCM_PRESCRIPTION_STOP_RESULT_DTO_H_
#define _TCM_PRESCRIPTION_STOP_RESULT_DTO_H_

// 引入项目通用定义和 DTO 宏
#include "../../GlobalInclude.h"

// 开始生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_BEGIN(DTO)

// 定义停嘱接口的业务返回结果
class TcmPrescriptionStopResultDTO : public oatpp::DTO
{
    // 初始化当前 DTO 类型
    DTO_INIT(TcmPrescriptionStopResultDTO, DTO);

    // 表示停嘱操作是否成功
    API_DTO_FIELD(Boolean, success, ZH_WORDS_GETTER("tcm-prescription.field.success"), true, true);

    // 表示停嘱操作的提示信息
    API_DTO_FIELD(String, message, ZH_WORDS_GETTER("tcm-prescription.field.message"), true, "operation completed");
};

// 结束生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_END(DTO)

// 结束头文件保护
#endif
