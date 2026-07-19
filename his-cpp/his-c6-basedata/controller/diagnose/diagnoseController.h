#pragma once
#ifndef _USERCONTROLLER_H_
#define _USERCONTROLLER_H_
#include "domain/query/user/UserQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/user/UserVO.h"
#include "domain/vo/user/MenuVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("diagnose.tag")

class DiagnoseController : public oatpp::web::server::api::ApiController
{
	// 添加访问定义
	API_ACCESS_DECLARE(DiagnoseController);
public:

	// 定义新增用户信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("user.add-user.summary"), addUser, StringJsonVO::Wrapper, API_TAG);
	// 定义新增用户信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/user/add-user", addUser, BODY_DTO(UserAddDTO::Wrapper, dto), executeAddUser(dto));

	// 定义新增收货地址接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("address.add-address.summary"), addUserAddr, StringJsonVO::Wrapper, API_TAG);
	// 定义新增收货地址接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/user/add-user-addr", addUserAddr, BODY_DTO(AddressAddDTO::Wrapper, dto), executeAddUserAddr(dto));

	// 定义查询用户菜单接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("user.query-menu.summary"), queryMenu, MenuJsonVO::Wrapper, API_TAG);
	// 定义查询用户菜单接口端点
	API_HANDLER_ENDPOINT_NOPARAM_AUTH(API_M_GET, "/user/query-menu", queryMenu, executeQueryMenu(authObject->getPayload()));
private:

	// 新增用户信息
	StringJsonVO::Wrapper executeAddUser(const UserAddDTO::Wrapper& dto);
	// 新增用户地址
	StringJsonVO::Wrapper executeAddUserAddr(const AddressAddDTO::Wrapper& dto);
	// 测试菜单
	MenuJsonVO::Wrapper executeQueryMenu(const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _USERCONTROLLER_H_