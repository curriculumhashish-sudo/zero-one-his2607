/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#include "stdafx.h"
#include "MedicalOrderVerificationController.h"
#include "service/medicalorderverification/MedicalOrderVerificationService.h"

MedicalOrderVerificationJsonVO::Wrapper MedicalOrderVerificationController::execQueryMedicalOrderVerification(
    const MedicalVerificationQuery::Wrapper& query) {
    auto vo=MedicalOrderVerificationJsonVO::createShared(); vo->success(MedicalOrderVerificationService().list(query)); return vo;
}

VerifyPassJsonVO::Wrapper MedicalOrderVerificationController::execVerifyPass(
    const VerifyPassRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
    auto jvo = VerifyPassJsonVO::createShared();
    auto result=MedicalOrderVerificationService().verify(dto,payload.getUsername()); result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

RejectOrdersJsonVO::Wrapper MedicalOrderVerificationController::execRejectOrders(
    const RejectOrdersRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
    auto jvo = RejectOrdersJsonVO::createShared();
    auto result=MedicalOrderVerificationService().reject(dto,payload.getUsername()); result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}
