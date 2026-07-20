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

// 3.1 定义新增接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.post.summary"), addDiagnose, StringJsonVO::Wrapper, API_TAG);
// 3.2 定义新增接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose", addDiagnose, BODY_DTO(DiagnoseAddDTO::Wrapper, dto), execAddDiagnose(dto, authObject->getPayload()));

// 3.1 定义新增接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.post.summary_category"), addDiagcategory, StringJsonVO::Wrapper, API_TAG);
// 3.2 定义新增接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose/category", addDiagcategory, BODY_DTO(DiagcategoryAddDTO::Wrapper, dto), execAddDiagcategory(dto, authObject->getPayload()));


	//  定义修改接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.put.summary"), modifyDiagnose, StringJsonVO::Wrapper, API_TAG);
// 定义修改接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnose", modifyDiagnose, BODY_DTO(SampleDTO::Wrapper, dto), execModifyDiagnose(dto, authObject->getPayload()));

//  定义删除接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summary"), removeDiagnose, ListJsonVO<String>::Wrapper, API_TAG);
//  定义删除接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnose", removeDiagnose, BODY_DTO(List<String>, ids), execRemoveDiagnose(ids));

private:

// 3.3 演示新增数据
StringJsonVO::Wrapper execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
// 3.3 演示新增数据
StringJsonVO::Wrapper execAddDiagcategory(const DiagcategoryAddDTO::Wrapper& dto, const PayloadDTO& payload);
// 3.3 演示修改数据
StringJsonVO::Wrapper execModifyDiagnose(const SampleDTO::Wrapper& dto, const PayloadDTO& payload);
// 3.3 演示删除数据
ListJsonVO<String>::Wrapper execRemoveDiagnose(const List<String>& ids);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _USERCONTROLLER_H_