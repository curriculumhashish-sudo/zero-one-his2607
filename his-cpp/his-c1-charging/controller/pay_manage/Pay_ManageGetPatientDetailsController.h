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

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pay_manage/Pay_ManageGetPatientDetailsQuery.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientDetailsDTO.h"
#include "domain/vo/pay_manage/Pay_ManageGetPatientDetailsVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.pay_manage.tag")

class Pay_ManageGetPatientDetailsController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(Pay_ManageGetPatientDetailsController);
public:

    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("charging.pay_manage.GetPatientDetails.summary"),
        Pay_ManageGetPatientDetails,
        Pay_ManageGetPatientDetailsQuery,
        Pay_ManageGetPatientDetailsVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/charging/pay_manage/getpatientdetails", Pay_ManageGetPatientDetails,
        Pay_ManageGetPatientDetailsQuery, execPay_ManageGetPatientDetails(query)
    );

private:
    Pay_ManageGetPatientDetailsVO::Wrapper execPay_ManageGetPatientDetails(const Pay_ManageGetPatientDetailsQuery::Wrapper& query);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
