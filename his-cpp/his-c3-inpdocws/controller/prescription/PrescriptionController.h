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
 * Prescription endpoints:
 *   - GET  /prescription/query-history
 *   - GET  /prescription/query-current
 *   - POST /prescription/add
 *   - DEL  /prescription/delete
 */
class PrescriptionController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PrescriptionController);
public:
	// Query historical prescriptions.
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription.query-history.summary"), queryPrescriptionHistory, PrescriptionHistoryQuery, PrescriptionHistoryListJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/prescription/query-history", queryPrescriptionHistory, PrescriptionHistoryQuery, execQueryHistory(query));

	// Query current prescriptions.
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription.query-current.summary"), queryPrescriptionCurrent, PrescriptionCurrentQuery, PrescriptionCurrentListJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/prescription/query-current", queryPrescriptionCurrent, PrescriptionCurrentQuery, execQueryCurrent(query));

	// Add a new prescription.
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.post.summary"), addPrescription, PrescriptionNewResponseVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/prescription/add", addPrescription, BODY_DTO(PrescriptionNewDTO::Wrapper, dto), execAddPrescription(dto, authObject->getPayload()));

	// Soft-delete prescriptions by medication request ids.
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.delete.summary"), removePrescription, PrescriptionDeleteResponseVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/prescription/delete", removePrescription, BODY_DTO(PrescriptionDeleteDTO::Wrapper, ids), execRemovePrescription(ids));
private:
	PrescriptionHistoryListJsonVO::Wrapper execQueryHistory(const PrescriptionHistoryQuery::Wrapper& query);
	PrescriptionCurrentListJsonVO::Wrapper execQueryCurrent(const PrescriptionCurrentQuery::Wrapper& query);
	PrescriptionNewResponseVO::Wrapper execAddPrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload);
	PrescriptionDeleteResponseVO::Wrapper execRemovePrescription(const PrescriptionDeleteDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRESCRIPTIONCONTROLLER_H_