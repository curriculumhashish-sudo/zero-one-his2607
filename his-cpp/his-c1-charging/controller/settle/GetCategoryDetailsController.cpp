#include "GetCategoryDetailsController.h"
#include "../../service/settle/GetCategoryDetailsService.h"


GetCategoryDetailsJsonVO::Wrapper GetCategoryDetailsController::execQueryGetCategoryDetails(const GetCategoryDetailsQuery::Wrapper& query)
{

    auto vo = GetCategoryDetailsJsonVO::createShared();
    GetCategoryDetailsService service;
    auto dtoList = service.listAll(query);
    vo->success(dtoList);

    return vo;
}

