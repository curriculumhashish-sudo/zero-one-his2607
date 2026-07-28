#ifndef _ORGANIZATIONQUERYERRORVO_H_
#define _ORGANIZATIONQUERYERRORVO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Unified failure envelope for the three organization query endpoints.
 *
 * The serialized structure is code/message/data with data set to null.
 * Current controller mappings are:
 * - HTTP 400, code 9995: invalid query parameter.
 * - HTTP 404, code 9999: organization detail not found.
 * - HTTP 500, code 9994: query processing failure.
 * Authentication failures remain owned by the global authentication layer.
 */
class OrganizationQueryErrorVO : public JsonVO<oatpp::String>
{
    DTO_INIT(OrganizationQueryErrorVO, JsonVO<oatpp::String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONQUERYERRORVO_H_
