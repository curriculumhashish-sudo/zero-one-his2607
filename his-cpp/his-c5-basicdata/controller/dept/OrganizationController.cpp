#include "stdafx.h"
#include "OrganizationController.h"

#include <cctype>

namespace
{
    constexpr std::size_t ORGANIZATION_ID_MAX_LENGTH = 64;
    constexpr std::size_t ORGANIZATION_ENUM_MAX_LENGTH = 255;
    constexpr std::size_t ORGANIZATION_KEYWORD_MAX_LENGTH = 500;

    bool exceedsLength(
        const oatpp::String& value,
        std::size_t maxLength
    )
    {
        return value && value->size() > maxLength;
    }

    bool isNameTreeQueryInvalid(
        const OrganizationNameTreeQuery::Wrapper& query
    )
    {
        return !query ||
            exceedsLength(query->typeEnum, ORGANIZATION_ENUM_MAX_LENGTH) ||
            exceedsLength(query->classEnum, ORGANIZATION_ENUM_MAX_LENGTH) ||
            exceedsLength(query->excludeId, ORGANIZATION_ID_MAX_LENGTH);
    }

    bool isListTreeQueryInvalid(
        const OrganizationListTreeQuery::Wrapper& query
    )
    {
        return !query ||
            exceedsLength(query->typeEnum, ORGANIZATION_ENUM_MAX_LENGTH) ||
            exceedsLength(query->classEnum, ORGANIZATION_ENUM_MAX_LENGTH) ||
            exceedsLength(query->statusEnum, ORGANIZATION_ENUM_MAX_LENGTH) ||
            exceedsLength(query->keyword, ORGANIZATION_KEYWORD_MAX_LENGTH);
    }

    std::string trim(const oatpp::String& value)
    {
        if (!value)
        {
            return "";
        }

        const std::string source = value.getValue("");
        std::size_t first = 0;

        while (first < source.size() &&
            std::isspace(static_cast<unsigned char>(source[first])))
        {
            ++first;
        }

        std::size_t last = source.size();

        while (last > first &&
            std::isspace(static_cast<unsigned char>(source[last - 1])))
        {
            --last;
        }

        return source.substr(first, last - first);
    }
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
OrganizationController::executeQueryNameTree(
    const OrganizationNameTreeQuery::Wrapper& query
)
{
    if (isNameTreeQueryInvalid(query))
    {
        return createErrorResponse(
            Status::CODE_400,
            RS_PARAMS_INVALID.getCode(),
            "dept.organizationQuery.errors.invalidParameter"
        );
    }

    try
    {
        OrganizationService service;
        return createDtoResponse(
            Status::CODE_200,
            service.getNameTree(query)
        );
    }
    catch (const std::exception& exception)
    {
        OATPP_LOGE(
            "OrganizationController",
            "Name-tree query failed: %s",
            exception.what()
        );
    }
    catch (...)
    {
        OATPP_LOGE(
            "OrganizationController",
            "Name-tree query failed with an unknown exception"
        );
    }

    return createErrorResponse(
        Status::CODE_500,
        RS_SERVER_ERROR.getCode(),
        "dept.organizationQuery.errors.serverError"
    );
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
OrganizationController::executeQueryListTree(
    const OrganizationListTreeQuery::Wrapper& query
)
{
    if (isListTreeQueryInvalid(query))
    {
        return createErrorResponse(
            Status::CODE_400,
            RS_PARAMS_INVALID.getCode(),
            "dept.organizationQuery.errors.invalidParameter"
        );
    }

    try
    {
        OrganizationService service;
        return createDtoResponse(
            Status::CODE_200,
            service.getListTree(query)
        );
    }
    catch (const std::exception& exception)
    {
        OATPP_LOGE(
            "OrganizationController",
            "List-tree query failed: %s",
            exception.what()
        );
    }
    catch (...)
    {
        OATPP_LOGE(
            "OrganizationController",
            "List-tree query failed with an unknown exception"
        );
    }

    return createErrorResponse(
        Status::CODE_500,
        RS_SERVER_ERROR.getCode(),
        "dept.organizationQuery.errors.serverError"
    );
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
OrganizationController::executeQueryDetail(
    const oatpp::String& id
)
{
    const std::string normalizedId = trim(id);

    if (normalizedId.empty() ||
        normalizedId.size() > ORGANIZATION_ID_MAX_LENGTH)
    {
        return createErrorResponse(
            Status::CODE_400,
            RS_PARAMS_INVALID.getCode(),
            "dept.organizationQuery.errors.invalidParameter"
        );
    }

    try
    {
        OrganizationService service;
        auto result = service.getDetail(normalizedId.c_str());

        if (!result || !result->data)
        {
            return createErrorResponse(
                Status::CODE_404,
                RS_FAIL.getCode(),
                "dept.organizationQuery.errors.notFound"
            );
        }

        return createDtoResponse(Status::CODE_200, result);
    }
    catch (const std::exception& exception)
    {
        OATPP_LOGE(
            "OrganizationController",
            "Detail query failed: %s",
            exception.what()
        );
    }
    catch (...)
    {
        OATPP_LOGE(
            "OrganizationController",
            "Detail query failed with an unknown exception"
        );
    }

    return createErrorResponse(
        Status::CODE_500,
        RS_SERVER_ERROR.getCode(),
        "dept.organizationQuery.errors.serverError"
    );
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
OrganizationController::createErrorResponse(
    const Status& status,
    int code,
    const std::string& messageKey
)
{
    auto result = OrganizationQueryErrorVO::createShared();
    result->code = code;
    result->message = ZH_WORDS_GETTER(messageKey);
    result->data = nullptr;

    return createDtoResponse(status, result);
}
