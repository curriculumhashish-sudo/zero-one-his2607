#include "stdafx.h"
#include "WardService.h"
#include "../../dao/ward/WardDAO.h"


 oatpp::List<WardNameVO::Wrapper> 
     WardService::listNames(const std::string& organizationId)
{
    WardDAO dao;

    auto rows = dao.selectNameList(organizationId);

    auto result =
        oatpp::List<WardNameVO::Wrapper>::createShared();

    for (const auto& row : rows)
    {
        auto vo = WardNameVO::createShared();

        vo->id = row.getId();
        vo->name = row.getName();

        result->push_back(vo);
    }

    return result;
}