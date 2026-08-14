#pragma once

#ifndef _PRESCRIPTION_SERVICE_
#define _PRESCRIPTION_SERVICE_

#include "../../domain/query/prescription/PrescriptionHistoryQuery.h"
#include "../../domain/query/prescription/PrescriptionCurrentQuery.h"
#include "../../domain/dto/prescription/PrescriptionHistoryListDTO.h"
#include "../../domain/dto/prescription/PrescriptionCurrentListDTO.h"
#include "../../domain/dto/prescription/PrescriptionNewDTO.h"
#include "../../domain/dto/prescription/PrescriptionNewResponseDTO.h"
#include "../../domain/dto/prescription/PrescriptionDeleteDTO.h"
#include "../../domain/dto/prescription/PrescriptionDeleteResponseDTO.h"
#include "domain/dto/PayloadDTO.h"

#include "json.hpp"
#include "RedisClient.h"

/**
 * Queries, inserts, and soft-deletes prescription records.
 */
class PrescriptionService
{
private:
	// Inject the shared Redis client registered in OtherComponent.hpp.
	//OATPP_COMPONENT(std::shared_ptr<RedisClient>, redisClient);

public:
	PrescriptionHistoryListDTO::Wrapper listAllHistoryPrescription(const PrescriptionHistoryQuery::Wrapper& query);
	PrescriptionCurrentListDTO::Wrapper listAllCurrentPrescription(const PrescriptionCurrentQuery::Wrapper& query);
	PrescriptionNewResponseDTO::Wrapper savePrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload);
	PrescriptionDeleteResponseDTO::Wrapper removePrescription(const PrescriptionDeleteDTO::Wrapper& dto);
};

#endif // !_PRESCRIPTION_SERVICE_