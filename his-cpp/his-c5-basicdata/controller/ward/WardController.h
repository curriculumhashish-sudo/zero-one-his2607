#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/07/23 18:58:00

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
#ifndef _WARD_CONTROLLER_H_
#define _WARD_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/ward/WardQuery.h"
#include "domain/query/ward/WardTreeQuery.h"
#include "domain/vo/ward/WardVO.h"
#include "domain/vo/ward/WardTreeNodeVO.h"
#include "domain/dto/ward/WardDTO.h"
#include "domain/dto/ward/WardModifyDTO.h"
#include "domain/dto/ward/WardStatusDTO.h"
#include "domain/dto/ward/WardLocationAddDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("ward.tag")

/**
 * WardController - Ward/room/bed/location management API controller.
 *
 * Provides endpoints for ward tree, room/bed query,
 * ward CRUD, status management, and location management.
 */
class WardController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(WardController);

public:
    // ============================================================
    // c5 endpoints: room/bed query, ward CRUD, status management
    // ============================================================

    // Query room by ID
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.query-room-by-id.summary"), queryRoomById, WardRoomJsonVO::Wrapper, API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.roomId"), "2233", true);
    );
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/patient/query-room-by-id", queryRoomById, QUERY(String, id), execQueryRoomById(id));

    // Query bed by ID
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.query-bed-by-id.summary"), queryBedById, WardBedJsonVO::Wrapper, API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.bedId"), "2233", true);
    );
    API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/ward/query-bed-by-id", queryBedById, QUERY(String, id), execQueryBedById(id));

    // Modify ward
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("ward.modify.summary"), modifyWard, WardJsonVO::Wrapper, API_TAG,
        info->description = "Modify ward basic information by ID. This endpoint returns mock success data for API documentation.";
        info->addConsumes<WardModifyDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/ward", modifyWard, BODY_DTO(WardModifyDTO::Wrapper, dto), execModifyWard(dto, authObject->getPayload()));

    // Remove ward
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("ward.remove.summary"), removeWard, BooleanJsonVO::Wrapper, API_TAG,
        info->description = "Remove ward by ID. This endpoint returns a mock success result for API documentation.";
        API_DEF_ADD_PATH_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.id"), "322955305339588608", true);
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/ward/{id}", removeWard, PATH(String, id, "id"), execRemoveWard(id, authObject->getPayload()));

    // Change ward status
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("ward.status.summary"), modifyWardStatus, WardJsonVO::Wrapper, API_TAG,
        info->description = "Enable or disable ward by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
        info->addConsumes<WardStatusDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/ward/status", modifyWardStatus, BODY_DTO(WardStatusDTO::Wrapper, dto), execModifyWardStatus(dto, authObject->getPayload()));

    // ============================================================
    // c5_tgg endpoints: ward tree list, location management
    // ============================================================

    // Query ward tree list
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("ward.tree-list.summary"),
        queryWardTree,
        WardTreeQuery,
        ListJsonVO<WardTreeNodeVO::Wrapper>::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/ward/tree-list",
        queryWardTree,
        WardTreeQuery,
        execWardTree(query)
    );

    // Add ward location
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.add-location.summary"),
        addWardLocation,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/ward/location",
        addWardLocation,
        BODY_DTO(WardLocationAddDTO::Wrapper, dto),
        execAddLocation(dto, authObject->getPayload())
    );

private:
    // ---- c5 method declarations ----

    WardRoomJsonVO::Wrapper execQueryRoomById(const String& id);
    WardBedJsonVO::Wrapper execQueryBedById(const String& id);

    WardJsonVO::Wrapper execModifyWard(
        const WardModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);

    BooleanJsonVO::Wrapper execRemoveWard(
        const String& id,
        const PayloadDTO& payload);

    WardJsonVO::Wrapper execModifyWardStatus(
        const WardStatusDTO::Wrapper& dto,
        const PayloadDTO& payload);

    // ---- c5_tgg method declarations ----

    ListJsonVO<WardTreeNodeVO::Wrapper>::Wrapper
        execWardTree(const WardTreeQuery::Wrapper& query);

    StringJsonVO::Wrapper execAddLocation(
        const WardLocationAddDTO::Wrapper& dto,
        const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _WARD_CONTROLLER_H_
