#pragma once
/*
 患者删除
*/
#ifndef _PAT_CONTROLLER_
#define _PAT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/pat/PatVO.h"
#include "domain/dto/pat/PatDTO.h"
#include "domain/query/pat/PatQuery.h"
#include "ApiHelper.h"
// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) 

#define API_TAG_PAT ZH_WORDS_GETTER("pat.tags")

class PatController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PatController);

public:
	// 删除患者（批量）: BODY -> List<String> ids
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("pat.delete.summary"), removePat, StringJsonVO::Wrapper, API_TAG_PAT);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/pat", removePat, BODY_DTO(List<String>, ids), execRemovePat(ids));

private:
	StringJsonVO::Wrapper execRemovePat(const List<String>& ids);
};

#undef API_TAG_PAT

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController) 

#endif // _PAT_CONTROLLER_