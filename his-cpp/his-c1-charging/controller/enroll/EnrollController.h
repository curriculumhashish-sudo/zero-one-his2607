#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/19

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

#include "domain/query/enroll/EnrollQuery.h"
#include "domain/vo/enroll/EnrollVO.h"
#include "service/enroll/EnrollService.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.enroll.tag")

/**
 * Inpatient registration interfaces.
 */

class EnrollController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(EnrollController);

public:
	// Define the paginated query interface document.
	API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("charging.enroll.query_all.summary"),
        queryAll,
        EnrollPageJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_PAGE_PARAMS();
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "keyword",
            ZH_WORDS_GETTER("QQQQ.field.keyword"),
            "Zhang San",
            false
        );
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "idCard",
            ZH_WORDS_GETTER("QQQQ.field.id_card"),
            "11010119900101123X",
            false
        );
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "registrationListStatuses",
            ZH_WORDS_GETTER("QQQQ.field.statuses"),
            "WAIT_ADMISSION",
            false
        );
    );

	// Bind the paginated query interface.
	API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/enroll/query-all",
        queryAll,
        EnrollQuery,
        execQueryAll(query)
    );

	// Define the detail query interface document.
	API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("charging.enroll.query_by_id.summary"),
        queryById,
        EnrollDetailJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "encounterId",
            ZH_WORDS_GETTER("QQQQ.field.encounter_id"),
            "demo-encounter-001",
            true
        );
    );

	// Bind the detail query interface.
	API_HANDLER_ENDPOINT_AUTH(
        API_M_GET,
        "/charging/enroll/query-by-id",
        queryById,
        QUERY(String, encounterId),
        execQueryById(encounterId)
    );

private:
	// Execute the paginated query.
	EnrollPageJsonVO::Wrapper execQueryAll(
        const EnrollQuery::Wrapper& query
    );

	// Execute the detail query.
	EnrollDetailJsonVO::Wrapper execQueryById(
        const String& encounterId
    );
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
