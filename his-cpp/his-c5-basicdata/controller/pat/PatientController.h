#pragma once

#ifndef _PATIENT_CONTROLLER_
#define _PATIENT_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
//#include "domain/query/pat/PatientQuery.h"
#include "domain/dto/pat/PatientDTO.h"
#include "domain/vo/pat/PatientVO.h"

// 0 定义API控制器使用宏
#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("patient.tag")

/**
 * 患者管理控制器
 */
class PatientController : public oatpp::web::server::api::ApiController // 1 继承控制器
{
	// 2 定义控制器访问入口
	API_ACCESS_DECLARE(PatientController);
	// 3 定义接口
public:
	// 3.1 定义ID查询接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("patient.query-by-id.summary"), queryPatientById, PatientJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("patient.field.id"), "2233", true);
	);
	// 3.2 定义ID查询接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/patient/query-by-id", queryPatientById, QUERY(String, id), execQueryById(id));

	// 3.1 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("patient.post.summary"), addPatient, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/patient", addPatient, BODY_DTO(PatientAddDTO::Wrapper, dto), execAddPatient(dto, authObject->getPayload()));
	 
	// 3.1 定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("patient.put.summary"), modifyPatient, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/patient", modifyPatient, BODY_DTO(PatientDTO::Wrapper, dto), execModifyPatient(dto, authObject->getPayload()));

private:
	// 3.3 演示ID查询数据
	PatientJsonVO::Wrapper execQueryById(const String& id);
	// 3.3 演示新增数据
	StringJsonVO::Wrapper execAddPatient(const PatientAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 演示修改数据
	StringJsonVO::Wrapper execModifyPatient(const PatientDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG

// 0 取消API控制器使用宏
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _PATIENT_CONTROLLER_