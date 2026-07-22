/*
 Copyright Zero One Star. All rights reserved.

 中药处方控制器接口定义

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

#ifndef _TCM_PRESCRIPTION_CONTROLLER_H_
#define _TCM_PRESCRIPTION_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/prescription/TcmPrescriptionSignDTO.h"
#include "domain/vo/prescription/TcmPrescriptionSignVO.h"
#include "domain/dto/prescription/TcmPrescriptionWithdrawDTO.h"
#include "domain/vo/prescription/TcmPrescriptionWithdrawVO.h"

// 引入停嘱请求 DTO
#include "domain/dto/prescription/TcmPrescriptionStopDTO.h"

// 引入停嘱响应 VO
#include "domain/vo/prescription/TcmPrescriptionStopVO.h"

// 引入取消停嘱请求 DTO
#include "domain/dto/prescription/TcmPrescriptionCancelStopDTO.h"

// 引入取消停嘱响应 VO
#include "domain/vo/prescription/TcmPrescriptionCancelStopVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("tcm-prescription.tags.tag")

/**
 * 中药处方控制器。
 * 当前阶段只负责定义请求参数、响应结构和 Swagger 文档，后续再接入 Service、DAO。
 */
class TcmPrescriptionController
    : public oatpp::web::server::api::ApiController
{
    // 注册控制器访问入口，供权限拦截器识别当前控制器。
    API_ACCESS_DECLARE(TcmPrescriptionController);

public:
    // 1. 定义签发接口的 Swagger 描述，展示接口标题、响应类型和接口分组。
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.sign.summary"),signTcmPrescription,TcmPrescriptionSignJsonVO::Wrapper,API_TAG);

    // 2. 定义签发接口的请求处理，使用 POST 接收处方签发请求并启用认证。
    API_HANDLER_ENDPOINT_AUTH(API_M_POST,"/tcm-prescription/sign",signTcmPrescription,BODY_DTO(TcmPrescriptionSignDTO::Wrapper, dto),execSignTcmPrescription(dto, authObject->getPayload()));

    // 1. 定义撤回接口的 Swagger 描述。
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.withdraw.summary"), withdrawTcmPrescription, TcmPrescriptionWithdrawJsonVO::Wrapper, API_TAG);

    // 2. 定义撤回接口的请求处理，接收撤回目标并返回撤回结果。
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/withdraw", withdrawTcmPrescription, BODY_DTO(TcmPrescriptionWithdrawDTO::Wrapper, dto), execWithdrawTcmPrescription(dto, authObject->getPayload()));

    // 1. 定义停嘱接口的 Swagger 描述。
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.stop.summary"), stopTcmPrescription, TcmPrescriptionStopJsonVO::Wrapper, API_TAG);

    // 2. 定义停嘱接口的请求处理，接收就诊、患者和处方编号列表。
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/stop", stopTcmPrescription, BODY_DTO(TcmPrescriptionStopDTO::Wrapper, dto), execStopTcmPrescription(dto, authObject->getPayload()));

    // 1. 定义取消停嘱接口的 Swagger 描述。
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("tcm-prescription.cancel-stop.summary"), cancelStopTcmPrescription, TcmPrescriptionCancelStopJsonVO::Wrapper, API_TAG);

    // 2. 定义取消停嘱接口的请求处理，接收需要恢复的处方编号列表。
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/tcm-prescription/cancel-stop", cancelStopTcmPrescription, BODY_DTO(TcmPrescriptionCancelStopDTO::Wrapper, dto), execCancelStopTcmPrescription(dto, authObject->getPayload()));
private:
    /**
     * 构造当前阶段的接口响应示例。
     * 后续由 Service 和 DAO 替换这里的测试返回逻辑。
     */
    // 执行停嘱接口的业务处理。
    TcmPrescriptionStopJsonVO::Wrapper execStopTcmPrescription(const TcmPrescriptionStopDTO::Wrapper& dto, const PayloadDTO& payload);

    // 执行取消停嘱接口的业务处理。
    TcmPrescriptionCancelStopJsonVO::Wrapper execCancelStopTcmPrescription(const TcmPrescriptionCancelStopDTO::Wrapper& dto, const PayloadDTO& payload);

    // 执行签发接口的业务处理。
    TcmPrescriptionSignJsonVO::Wrapper execSignTcmPrescription(const TcmPrescriptionSignDTO::Wrapper& dto,const PayloadDTO& payload);

    // 执行撤回接口的业务处理。
    TcmPrescriptionWithdrawJsonVO::Wrapper execWithdrawTcmPrescription(const TcmPrescriptionWithdrawDTO::Wrapper& dto, const PayloadDTO& payload);
};
    
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_TCM_PRESCRIPTION_CONTROLLER_H_
