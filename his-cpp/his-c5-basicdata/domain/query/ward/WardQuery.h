#pragma once
#ifndef _WARD_QUERY_
#define _WARD_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * WardRoomQuery - Query parameters for room list by ward area.
 */
class WardRoomQuery : public PageQuery {
    DTO_INIT(WardRoomQuery, PageQuery);
    // Ward area ID for filtering
    DTO_FIELD(String, wardAreaId);
    DTO_FIELD_INFO(wardAreaId) { info->description = ZH_WORDS_GETTER("ward.field.wardAreaId"); }
    // Optional name fuzzy search
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("ward.field.name"); }
};

/**
 * WardBedQuery - Query parameters for bed list by room.
 */
class WardBedQuery : public PageQuery {
    DTO_INIT(WardBedQuery, PageQuery);
    // Belonging room ID (required)
    DTO_FIELD(String, wardRoomId);
    DTO_FIELD_INFO(wardRoomId) { info->description = ZH_WORDS_GETTER("ward.field.wardRoomId"); }
    // Optional status filter
    DTO_FIELD(Int32, status);
    DTO_FIELD_INFO(status) { info->description = ZH_WORDS_GETTER("ward.field.status"); }
};

/**
 * WardQuery - Basic ward query parameters.
 */
class WardQuery : public oatpp::DTO
{
    DTO_INIT(WardQuery, DTO);
    // Organization ID (required)
    DTO_FIELD(String, organizationId);
    DTO_FIELD_INFO(organizationId) {
        info->description = ZH_WORDS_GETTER("ward.field.organization-id");
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _WARD_QUERY_
