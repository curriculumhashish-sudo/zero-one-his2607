/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/08/09

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
#ifndef _DOCTOR_STATION_CONTROLLER_H_
#define _DOCTOR_STATION_CONTROLLER_H_

#include "domain/vo/doctor2/InpatientAdmissionFormOptionsJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("doctor2.tags.tag")

class DoctorStationController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(DoctorStationController);

public:
    // Load inpatient admission form fixed options
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("doctor2.options.summary"),
        loadInpatientAdmissionOptions,
        InpatientAdmissionFormOptionsJsonVO::Wrapper,
        API_TAG);
    API_HANDLER_ENDPOINT_NOPARAM_AUTH(API_M_POST,
        "/doctor-station/inpatient-admission/options",
        loadInpatientAdmissionOptions,
        execLoadInpatientAdmissionOptions(authObject->getPayload()));

private:
    // Execute load inpatient admission options
    InpatientAdmissionFormOptionsJsonVO::Wrapper execLoadInpatientAdmissionOptions(
        const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_DOCTOR_STATION_CONTROLLER_H_
