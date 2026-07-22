#pragma once

#ifndef _PRESCRIPTIONCONTROLLER_H_
#define _PRESCRIPTIONCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/prescription/PrescriptionHistoryQuery.h"
#include "domain/dto/prescription/PrescriptionHistoryDTO.h"
#include "domain/dto/prescription/PrescriptionHistoryListDTO.h"
#include "domain/vo/prescription/PrescriptionHistoryVO.h"

#include "domain/query/prescription/PrescriptionCurrentQuery.h"
#include "domain/dto/prescription/PrescriptionCurrentMedicineDTO.h"
#include "domain/dto/prescription/PrescriptionCurrentDTO.h"
#include "domain/dto/prescription/PrescriptionCurrentListDTO.h"
#include "domain/vo/prescription/PrescriptionCurrentVO.h"

#include "domain/dto/prescription/PrescriptionNewMedicineDTO.h"
#include "domain/dto/prescription/PrescriptionNewPrDTO.h"
#include "domain/dto/prescription/PrescriptionNewDTO.h"
#include "domain/dto/prescription/PrescriptionNewResponseDTO.h"
#include "domain/vo/prescription/PrescriptionNewResponseVO.h"

#include "domain/dto/prescription/PrescriptionDeleteDTO.h"
#include "domain/dto/prescription/PrescriptionDeleteResponseDTO.h"
#include "domain/vo/prescription/PrescriptionDeleteResponseVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("prescription.tag")

/**
* 定义处方接口
* （一共有4个接口）
*/
class PrescriptionController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(PrescriptionController);
public:
	// 1.1定义历史处方查询接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription.query-history.summary"), queryPrescriptionHistory, PrescriptionHistoryQuery,PrescriptionHistoryListJsonVO::Wrapper, API_TAG);
	// 1.2 定义历史处方查询接口定义
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET,"/prescription/query-history", queryPrescriptionHistory, PrescriptionHistoryQuery, execQueryHistory(query));

	// 2.1定义现在处方查询接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription.query-current.summary"), queryPrescriptionCurrent, PrescriptionCurrentQuery,PrescriptionCurrentListJsonVO::Wrapper, API_TAG);
	// 2.2 定义现在处方查询接口定义
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/prescription/query-current", queryPrescriptionCurrent, PrescriptionCurrentQuery,execQueryCurrent(query));

	// 3.1 定义新增处方描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.post.summary"), addPrescription, PrescriptionNewResponseVO::Wrapper, API_TAG);
	// 3.2 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/prescription/add", addPrescription, BODY_DTO(PrescriptionNewDTO::Wrapper, dto), execAddPrescription(dto, authObject->getPayload()));

	// 4.1 定义删除处方描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.delete.summary"), removePrescription, PrescriptionDeleteResponseVO::Wrapper, API_TAG);
	// 4.2 定义删除处方处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/prescription/delete", removePrescription, BODY_DTO(PrescriptionDeleteDTO::Wrapper, ids), execRemovePrescription(ids));
private:
	//定义历史处方执行函数
	PrescriptionHistoryListJsonVO::Wrapper execQueryHistory(const PrescriptionHistoryQuery::Wrapper& query);
	//定义现在处方执行函数
	PrescriptionCurrentListJsonVO::Wrapper execQueryCurrent(const PrescriptionCurrentQuery::Wrapper& query);
	//定义新增处方接收函数
	PrescriptionNewResponseVO::Wrapper execAddPrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload);
	//定义删除处方执行函数
	PrescriptionDeleteResponseVO::Wrapper execRemovePrescription(const PrescriptionDeleteDTO::Wrapper& ids);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRESCRIPTIONCONTROLLER_H_