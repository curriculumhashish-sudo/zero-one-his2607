/*
 Copyright Zero One Star. All rights reserved.

 签发中药处方结果 DTO

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

#ifndef _TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_
#define _TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Define the traditional Chinese medicine prescription signing result.
 */
class TcmPrescriptionSignResultDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignResultDTO, DTO);
    
    // Indicate whether the prescription signing operation succeeded.
    API_DTO_FIELD(Boolean,success,ZH_WORDS_GETTER("tcm-prescription.field.success"),true,true);
    // Map each prescription group key to its generated prescription number.
    API_DTO_FIELD(Fields<String>, prescriptionNoByGroupKey, ZH_WORDS_GETTER("tcm-prescription.field.prescription-no-by-group-key"), true, {});
public:
    /**
     * Initialize response example values for Swagger.
     */
    TcmPrescriptionSignResultDTO()
    {
        this->success = true;
        this->prescriptionNoByGroupKey = {
            {
                "tcm-rx-1782719488981-gejhfw",
                "CP2026-06-290003"
            }
        };
    }
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_
