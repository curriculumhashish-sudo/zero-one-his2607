#pragma once
#ifndef _DIAGNOSISCONTROLLER_H_
#define _DIAGNOSISCONTROLLER_H_


#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/diagnosis/DoctorDiagnosisAddDTO.h"
#include "domain/dto/diagnosis/DoctorDiagnosisModifyDTO .h"
#include "domain/dto/diagnosis/TCMDiagnosisAddDTO.h"
#include "domain/dto/diagnosis/TCMDiagnosisModifyDTO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnosis.tags.t1")
#define API_TAG2 ZH_WORDS_GETTER("diagnosis.tags.t2")

class DiagnosisController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisController);

public:
	// 3.1 定义西医新增诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.add.summary"), addDoctorDiagnosis, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义西医新增诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnosis/add-doctor", addDoctorDiagnosis, BODY_DTO(DoctorDiagnosisAddDTO::Wrapper, dto), execAddDoctorDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义西医修改诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.modify.summary"), modifyDoctorDiagnosis, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义西医修改诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnosis/modify-doctor", modifyDoctorDiagnosis, BODY_DTO(DoctorDiagnosisModifyDTO::Wrapper, dto), execModifyDoctorDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义西医删除诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.remove.summary"), removeDoctorDiagnosis, BooleanJsonVO::Wrapper, API_TAG);
	// 3.2 定义西医删除诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnosis/doctor/{id}", removeDoctorDiagnosis, PATH(String, id), execRemoveDoctorDiagnosis(id));




	// 3.1 定义中医新增诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.add.summary"), addTCMDiagnosis, StringJsonVO::Wrapper, API_TAG2);
	// 3.2 定义中医新增诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnosis/add-tcm", addTCMDiagnosis, BODY_DTO(TCMDiagnosisAddDTO::Wrapper, dto), execAddTCMDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义中医修改诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.modify.summary"), modifyTCMDiagnosis, StringJsonVO::Wrapper, API_TAG2);
	// 3.2 定义中医修改诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnosis/modify-tcm", modifyTCMDiagnosis, BODY_DTO(TCMDiagnosisModifyDTO::Wrapper, dto), execModifyTCMDiagnosis(dto, authObject->getPayload()));

	// 3.1 定义中医删除诊断接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.remove.summary"), removeTCMDiagnosis, BooleanJsonVO::Wrapper, API_TAG2);
	// 3.2 定义中医删除诊断接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnosis/tcm/{id}", removeTCMDiagnosis, PATH(String, id), execRemoveTCMDiagnosis(id));
private:
	StringJsonVO::Wrapper execAddDoctorDiagnosis(const DoctorDiagnosisAddDTO::Wrapper &dto,const PayloadDTO &payload);

	StringJsonVO::Wrapper execModifyDoctorDiagnosis(const DoctorDiagnosisModifyDTO::Wrapper& dto, const PayloadDTO& payload);

	BooleanJsonVO::Wrapper execRemoveDoctorDiagnosis(const String& id);

	StringJsonVO::Wrapper execAddTCMDiagnosis(const TCMDiagnosisAddDTO::Wrapper& dto, const PayloadDTO& payload);

	StringJsonVO::Wrapper execModifyTCMDiagnosis(const TCMDiagnosisModifyDTO::Wrapper& dto, const PayloadDTO& payload);

	BooleanJsonVO::Wrapper execRemoveTCMDiagnosis(const String& id);

};

#undef API_TAG
#undef API_TAG2
#include OATPP_CODEGEN_END(ApiController)






#endif // ! _DIAGNOSISCONTROLLER_H_
