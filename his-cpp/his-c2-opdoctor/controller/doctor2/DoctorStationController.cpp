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
#include "stdafx.h"
#include "DoctorStationController.h"
#include "../../service/doctor2/DoctorStationService.h"

InpatientAdmissionFormOptionsJsonVO::Wrapper
DoctorStationController::execLoadInpatientAdmissionOptions(const PayloadDTO& payload)
{
    auto jvo = InpatientAdmissionFormOptionsJsonVO::createShared();

    DoctorStationService service;
    auto data = service.loadInpatientAdmissionOptions(payload);

    jvo->success(data);
    return jvo;
}
