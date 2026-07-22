#pragma once
#ifndef _NURSECONTROLLER_H_
#define _NURSECONTROLLER_H_

#include "domain/query/nurse2/NurseQuery2.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/nurse2/NurseVO2.h"
#include "../../domain/dto/nurse2/NurseDetailDTO2.h"
//#include "../../domain/dto/nurse/NurseCancelDTO2.h"
#include "domain/dto/nurse2/NurseCancelDTO2.h"
//#include "../../domain/dto/nurse/NurseOperateDTO2.h"
#include "domain/dto/nurse2/NurseOperateDTO2.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("nurse.tags.tag")

class NurseController2:public oatpp::web::server::api::ApiController
{
	//定义控制器访问接口
	API_ACCESS_DECLARE(NurseController2);

public:  //定义接口   从 arch-demo下的/controller/samplecontroller。h   3.1 定义导出接口描述 3.2 定义导出接口描述
/*
	// 3.1 定义分页获取患者队列接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("nurse.query.summary"), queryNurse, NurseQuery2, NurseDetailPageJsonVO2::Wrapper, API_TAG);
	// 3.2  定义分页获取患者队列接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/nurse/query", queryNurse, NurseQuery2, executeQueryNurse(query));

	// 3.1 定义删除接口描述  ---> 这些在arch-demo下的/controller/samplecontroller。h
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse.remove.summary"), removeSample, BooleanJsonVO::Wrapper, API_TAG);
	// 3.2 定义删除接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/nurse/{id}", removeSample, BODY_DTO(String, id), execRemoveNurse(id));
	*/

	// 3.1 定义修改接口描述  --->也就是执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse2.modify.operate1"), operateNurse2, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/nurse/1", operateNurse2, BODY_DTO(NurseOperateDTO2::Wrapper,dto), execOperateNurse(dto, authObject->getPayload()));
	

	// 3.1 定义修改接口描述  --->也就是取消执行接口
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("nurse2.modify.operate2"), cancelNurse2, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/nurse/2", cancelNurse2, BODY_DTO(NurseCancelDTO2::Wrapper,dto), execCancelNurse(dto,authObject->getPayload()));

private://定义接口执行函数
	//3.3定义定义分页获取患者队列接口处理
	NurseDetailPageJsonVO2::Wrapper executeQueryNurse(const NurseQuery2::Wrapper& query);
	
	//定义删除接口执行函数
	BooleanJsonVO::Wrapper execRemoveNurse(const String& id);

	// 3.3 演示修改数据  ---> 执行 取消执行
	StringJsonVO::Wrapper execOperateNurse(const NurseOperateDTO2::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execCancelNurse(const NurseCancelDTO2::Wrapper& dto, const PayloadDTO& payload);

};

#undef API_TAG
#include  OATPP_CODEGEN_END(ApiController)


#endif // !_NURSECONTROLLER_H_
