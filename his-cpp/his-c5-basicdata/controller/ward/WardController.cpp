#include "stdafx.h"
#include "WardController.h"
#include "service/ward/WardService.h"

//∑µªÿ¿‡–Õ

ListJsonVO<WardNameVO::Wrapper>::Wrapper
WardController::execWardNameList(const WardQuery::Wrapper& query)
{
    // 1. Create response object.
     auto ljv=ListJsonVO<WardNameVO::Wrapper>::createShared();
    // 2. Validate organizationId.
     if (!query || query->organizationId || query->organizationId.getValue("").empty()) {
         ljv->init(nullptr, RS_PARAMS_INVALID);
         return ljv;
     }
    // 3. Call WardService.
     auto data = WardService().listNames(
         query->organizationId.getValue("")
         );
    // 4. Set success response.
     ljv->success(data);
    // 5. Return response.
     return ljv;
}