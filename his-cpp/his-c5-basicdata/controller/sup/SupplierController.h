#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: shelly
 @Date: 2026/7/20

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

#ifndef _SUPLIER_CONTROLLER_
#define _SUPLIER_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/sup/SupplierDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("sup.tag")

/**
 * SupplierController - 供应商批量启/停用 & 批量删除
 *
 * 新增/修改功能由 SupplierAddUpdateController 提供
 */
class SupplierController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(SupplierController);

public:
    // PUT /sup/batch-enable-disable — 批量启用/停用
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-enable-disable.summary"),
        batchEnableDisable,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/sup/batch-enable-disable",
        batchEnableDisable,
        BODY_DTO(SupplierBatchEnableDisableDTO::Wrapper, dto),
        execBatchEnableDisable(dto)
    );

    // DELETE /sup/batch-remove — 批量删除
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-remove.summary"),
        batchRemove,
        ListJsonVO<String>::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/sup/batch-remove",
        batchRemove,
        BODY_DTO(List<String>, ids),
        execBatchRemove(ids)
    );

private:
    StringJsonVO::Wrapper execBatchEnableDisable(const SupplierBatchEnableDisableDTO::Wrapper& dto);
    ListJsonVO<String>::Wrapper execBatchRemove(const List<String>& ids);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_SUPPLIER_CONTROLLER_