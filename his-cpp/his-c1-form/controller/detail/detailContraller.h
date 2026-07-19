#pragma once

#ifndef DETAIL_CONTRALLER_H
#define DETAIL_CONTRALLER_H
#include "domain/query/detail/detailQuery.h"
#include "domain/vo/detail/queryDetailVO.h"
#include "domain/dto/detail/queryDetailDTO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("user.tag")

class DetailContraller : public oatpp::web::server::api::ApiController
{
	// 添加访问定义
	API_ACCESS_DECLARE(DetailContraller);
public:
	// 定义查询所有用户信息接口描述(到这里) =======================================================
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("rhqm.form.detail.attain.title_1"), queryByName, DetailQuery, UserPageJsonVO::Wrapper, API_TAG);
	// 定义查询所有用户信息接口端点
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/user/query-all", queryAllUser, UserQuery, executeQueryAll(query));
private:
	// 查询所有
	UserPageJsonVO::Wrapper executeQueryAll(const UserQuery::Wrapper& userQuery);
	// 查询详情
	UserDetailJsonVO::Wrapper executeQueryOne(const String& id);
	// 新增用户信息
	StringJsonVO::Wrapper executeAddUser(const UserAddDTO::Wrapper& dto);
	// 新增用户地址
	StringJsonVO::Wrapper executeAddUserAddr(const AddressAddDTO::Wrapper& dto);
	// 测试菜单
	MenuJsonVO::Wrapper executeQueryMenu(const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) 

#endif // DETAIL_CONTRALLER_H