#pragma once
#ifndef _QUIT_NUMBER_CONTROLLER_H_
#define _QUIT_NUMBER_CONTROLLER_H_
#include "../../domain/dto/register/QuitNumberDTO.h"
#include "../../domain/vo/register/QuitNumberVO.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("macchiato.charging.register.tag.tag2")

class QuitNumberController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(QuitNumberController);
public:
    // 确认退号接口描述
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("macchiato.charging.register.summary2"), 
        confirmQuitNumber,                            
        QuitNumberJsonVO::Wrapper,
        API_TAG                                      
    );

    // 确认退号接口端点
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,                                   // HTTP方法：POST
        "/quit/number",                               // 路由路径
        confirmQuitNumber,                            // 端点名称
        BODY_DTO(QuitNumberDTO::Wrapper, dto),          // 请求体参数（QuitNumberDTO的Wrapper）
        execConfirmQuitNumber(dto, authObject->getPayload()) // 处理方法（传入DTO和用户身份）
    );

private:
    // 执行确认退号的核心逻辑
    QuitNumberJsonVO::Wrapper execConfirmQuitNumber(const QuitNumberDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_QUIT_NUMBER_CONTROLLER_H_

