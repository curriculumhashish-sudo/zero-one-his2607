#include "stdafx.h"
#include "OrganizationController.h"

OrganizationNameTreeJsonVO::Wrapper
OrganizationController::executeQueryNameTree(
    const OrganizationNameTreeQuery::Wrapper& query
)
{
    OrganizationService service;
    return service.getNameTree(query);
}

OrganizationListTreeJsonVO::Wrapper
OrganizationController::executeQueryListTree(
    const OrganizationListTreeQuery::Wrapper& query
)
{
    OrganizationService service;
    return service.getListTree(query);
}

OrganizationDetailJsonVO::Wrapper
OrganizationController::executeQueryDetail(
    const oatpp::String& id
)
{
    OrganizationService service;
    return service.getDetail(id);
}