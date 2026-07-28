#pragma once
#ifndef _DEPT_CONTROLLER_
#define _DEPT_CONTROLLER_

#include "ApiHelper.h"
#include "domain/dto/dept/deptDTO.h"
#include "domain/query/dept/DeptQuery.h"
#include "domain/vo/dept/DeptVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("dept.tag")

class DeptController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(DeptController);

public:
    // 定义新增接口描述
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.add.summary"),
        addDept,
        DeptAddJsonVO::Wrapper,
        API_TAG,
        info->addConsumes<DeptAddDTO::Wrapper>("application/json");
        info->addResponse<DeptAddJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<DeptAddJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<DeptAddJsonVO::Wrapper>(Status::CODE_409, "application/json");
        info->addResponse<DeptAddJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    // 定义新增接口处理
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_POST,
        "/dept/createDept",
        addDept,
        BODY_DTO(DeptAddDTO::Wrapper, dto),
        return execAddDept(dto, authObject->getPayload());
    );

    // 定义修改接口描述
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.modify.summary"),
        modifyDept,
        DeptModifyJsonVO::Wrapper,
        API_TAG,
        info->addConsumes<DeptModifyDTO::Wrapper>("application/json");
        info->addResponse<DeptModifyJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<DeptModifyJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<DeptModifyJsonVO::Wrapper>(Status::CODE_404, "application/json");
        info->addResponse<DeptModifyJsonVO::Wrapper>(Status::CODE_409, "application/json");
        info->addResponse<DeptModifyJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    // 定义修改接口处理
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_PUT,
        "/dept/editDept",
        modifyDept,
        BODY_DTO(DeptModifyDTO::Wrapper, dto),
        return execModifyDept(dto, authObject->getPayload());
    );

    // 定义删除接口描述
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.remove.summary"),
        removeDept,
        DeptDeleteJsonVO::Wrapper,
        API_TAG,
        info->addConsumes<DeptDeleteQuery::Wrapper>("application/json");
        info->addResponse<DeptDeleteJsonVO::Wrapper>(Status::CODE_400, "application/json");
        info->addResponse<DeptDeleteJsonVO::Wrapper>(Status::CODE_403, "application/json");
        info->addResponse<DeptDeleteJsonVO::Wrapper>(Status::CODE_404, "application/json");
        info->addResponse<DeptDeleteJsonVO::Wrapper>(Status::CODE_409, "application/json");
        info->addResponse<DeptDeleteJsonVO::Wrapper>(Status::CODE_500, "application/json");
    );
    // 定义删除接口处理
    API_HANDLER_ENDPOINT_OPTION_AUTH(
        API_M_DEL,
        "/dept/deleteDept",
        removeDept,
        BODY_DTO(DeptDeleteQuery::Wrapper, query),
        return execRemoveDept(query, authObject->getPayload());
    );

private:
    std::shared_ptr<OutgoingResponse> execAddDept(
        const DeptAddDTO::Wrapper& dto,
        const PayloadDTO& payload);
    std::shared_ptr<OutgoingResponse> execModifyDept(
        const DeptModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);
    std::shared_ptr<OutgoingResponse> execRemoveDept(
        const DeptDeleteQuery::Wrapper& query,
        const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DEPT_CONTROLLER_
