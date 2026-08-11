#pragma once
#ifndef _DIAGNOSISCONTROLLER_H_
#define _DIAGNOSISCONTROLLER_H_


#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/diagnosis/DiagnosisAddDTO.h"
#include "domain/dto/diagnosis/DiagnosisModifyDTO .h"
#include "domain/dto/diagnosis/DiagnosisRemoveDTO .h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnosis.tags.t1")


class DiagnosisController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisController);

public:
	// 3.1 定义新增诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.add.summary"), addDiagnosis, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义新增诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnosis/add", addDiagnosis, BODY_DTO(DiagnosisAddDTO::Wrapper, dto), execAddDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义修改诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.modify.summary"), modifyDiagnosis, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnosis/modify", modifyDiagnosis, BODY_DTO(DiagnosisModifyDTO::Wrapper, dto), execModifyDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义删除诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.remove.summary"), removeDiagnosis, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义删除诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnosis/remove", removeDiagnosis, BODY_DTO(DiagnosisRemoveDTO::Wrapper, dto), execRemoveDiagnosis(dto, authObject->getPayload()));

	


	
private:
	StringJsonVO::Wrapper execAddDiagnosis(const DiagnosisAddDTO::Wrapper &dto,const PayloadDTO &payload);

	StringJsonVO::Wrapper execModifyDiagnosis(const DiagnosisModifyDTO::Wrapper& dto, const PayloadDTO& payload);

	StringJsonVO::Wrapper execRemoveDiagnosis(const DiagnosisRemoveDTO::Wrapper& dto, const PayloadDTO& payload);

	

};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)






#endif // ! _DIAGNOSISCONTROLLER_H_
