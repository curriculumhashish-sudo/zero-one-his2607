/*
 Copyright Zero One Star. All rights reserved.

 取消停嘱响应 VO

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
#ifndef _TCM_PRESCRIPTION_CANCEL_STOP_VO_H_
#define _TCM_PRESCRIPTION_CANCEL_STOP_VO_H_

// 引入项目通用定义
#include "../../GlobalInclude.h"

// 引入取消停嘱接口的业务返回 DTO
#include "../../dto/prescription/TcmPrescriptionCancelStopResultDTO.h"

// 开始生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_BEGIN(DTO)

// 定义取消停嘱接口统一 JSON 返回对象
class TcmPrescriptionCancelStopJsonVO
    : public JsonVO<TcmPrescriptionCancelStopResultDTO::Wrapper>
{
    // 初始化 JSON 返回对象类型
    DTO_INIT(
        TcmPrescriptionCancelStopJsonVO,
        JsonVO<TcmPrescriptionCancelStopResultDTO::Wrapper>
    );
};

// 结束生成 Oatpp DTO 相关代码
#include OATPP_CODEGEN_END(DTO)

// 结束头文件保护
#endif
