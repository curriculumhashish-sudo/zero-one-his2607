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
#include "DoctorStationService.h"

InpatientAdmissionFormOptionsDTO::Wrapper DoctorStationService::loadInpatientAdmissionOptions(
    const PayloadDTO& payload)
{
    auto dto = InpatientAdmissionFormOptionsDTO::createShared();

    // Department options (with code field populated)
    auto deptList = oatpp::List<OptionItemDTO::Wrapper>::createShared();
    {
        auto item = OptionItemDTO::createShared();
        item->value = "inpatient-dept-cardio";
        item->label = "\u5FC3\u8840\u7BA1\u5185\u79D1";
        item->code = "I-CARDIO";
        deptList->push_back(item);
    }
    dto->departmentOptions = deptList;

    // Admission type options
    auto typeList = oatpp::List<OptionItemDTO::Wrapper>::createShared();
    {
        auto item = OptionItemDTO::createShared();
        item->value = "ELECTIVE";
        item->label = "\u62E9\u671F\u4F4F\u9662";
        item->code = nullptr;
        typeList->push_back(item);
    }
    {
        auto item = OptionItemDTO::createShared();
        item->value = "EMERGENCY";
        item->label = "\u6025\u8BCA\u4F4F\u9662";
        item->code = nullptr;
        typeList->push_back(item);
    }
    dto->admissionTypeOptions = typeList;

    // Admission way options
    auto wayList = oatpp::List<OptionItemDTO::Wrapper>::createShared();
    {
        auto item = OptionItemDTO::createShared();
        item->value = "OUTPATIENT_TRANSFER";
        item->label = "\u95E8\u8BCA\u8F6C\u4F4F\u9662";
        item->code = nullptr;
        wayList->push_back(item);
    }
    {
        auto item = OptionItemDTO::createShared();
        item->value = "EMERGENCY_TRANSFER";
        item->label = "\u6025\u8BCA\u8F6C\u4F4F\u9662";
        item->code = nullptr;
        wayList->push_back(item);
    }
    {
        auto item = OptionItemDTO::createShared();
        item->value = "TRANSFER_IN";
        item->label = "\u5916\u9662\u8F6C\u5165";
        item->code = nullptr;
        wayList->push_back(item);
    }
    dto->admissionWayOptions = wayList;

    // Priority options
    auto prioList = oatpp::List<OptionItemDTO::Wrapper>::createShared();
    {
        auto item = OptionItemDTO::createShared();
        item->value = "NORMAL";
        item->label = "\u666E\u901A";
        item->code = nullptr;
        prioList->push_back(item);
    }
    {
        auto item = OptionItemDTO::createShared();
        item->value = "URGENT";
        item->label = "\u7D27\u6025";
        item->code = nullptr;
        prioList->push_back(item);
    }
    {
        auto item = OptionItemDTO::createShared();
        item->value = "CRITICAL";
        item->label = "\u5371\u91CD";
        item->code = nullptr;
        prioList->push_back(item);
    }
    dto->priorityOptions = prioList;

    return dto;
}