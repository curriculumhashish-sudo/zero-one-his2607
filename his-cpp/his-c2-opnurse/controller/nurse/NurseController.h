#pragma once
#ifndef _NURSECONTROLLER_H_
#define _NURSECONTROLLER_H_

#include "domain/query/nurse/NurseQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/nurse/NurseVO.h"
#include "../../domain/dto/nurse/NurseDetailDTO.h"
//#include "../../domain/dto/nurse/NurseCancelDTO.h"
#include "domain/dto/nurse/NurseCancelDTO.h"
//#include "../../domain/dto/nurse/NurseOperateDTO.h"
#include "domain/dto/nurse/NurseOperateDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("nurse.tags.tag")

class NurseController:public oatpp::web::server::api::ApiController
{
	//定义控制器访问接口
	API_ACCESS_DECLARE(NurseController);

public:  //定义接口   从 arch-demo下的/controller/samplecontroller。h   3.1 定义导出接口描述 3.2 定义导出接口描述
/*
	// 3.1 定义分页获取患者队列接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("nurse.query.summary"), queryNurse, NurseQuery, NurseDetailPageJsonVO::Wrapper, API_TAG);
	// 3.2  定义分页获取患者队列接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/nurse/query", queryNurse, NurseQuery, executeQueryNurse(query));

	// 3.1 定义删除接口描述  ---> 这些在arch-demo下的/controller/samplecontroller。h
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse.remove.summary"), removeSample, BooleanJsonVO::Wrapper, API_TAG);
	// 3.2 定义删除接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/nurse/{id}", removeSample, BODY_DTO(String, id), execRemoveNurse(id));
	*/

	// 3.1 定义修改接口描述  --->也就是执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse.modify.operate1"), operateNurse, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/nurse/1", operateNurse, BODY_DTO(NurseOperateDTO::Wrapper,dto), execOperateNurse(dto, authObject->getPayload()));
	

	// 3.1 定义修改接口描述  --->也就是取消执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse.modify.operate2"), cancelNurse, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/nurse/2", cancelNurse, BODY_DTO(NurseCancelDTO::Wrapper,dto), execCancelNurse(dto,authObject->getPayload()));

private://定义接口执行函数
	//3.3定义定义分页获取患者队列接口处理
	NurseDetailPageJsonVO::Wrapper executeQueryNurse(const NurseQuery::Wrapper& query);
	
	//定义删除接口执行函数
	BooleanJsonVO::Wrapper execRemoveNurse(const String& id);

	// 3.3 演示修改数据  ---> 执行 取消执行
	StringJsonVO::Wrapper execOperateNurse(const NurseOperateDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execCancelNurse(const NurseCancelDTO::Wrapper& dto, const PayloadDTO& payload);

};

#undef API_TAG
#include  OATPP_CODEGEN_END(ApiController)


#endif // !_NURSECONTROLLER_H_
