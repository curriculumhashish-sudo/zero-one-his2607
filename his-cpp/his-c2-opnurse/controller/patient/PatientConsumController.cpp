/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/24 18:30:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "PatientConsumController.h"
#include "service/patient/PatientConsumService.h"

PatientConsumListJsonVO::Wrapper PatientConsumController::execConsumPatient(
	const PatientConsumQuery::Wrapper& query)
{
	// --- Extract parameters ---
	std::string patientId = query->patientId ? query->patientId.getValue("") : "";
	std::string encounterId = query->encounterId ? query->encounterId.getValue("") : "";
	std::string consumableRoom = query->consumableRoom ? query->consumableRoom.getValue("") : "";

	// --- Validate input ---
	if (patientId.empty()) {
		auto vo = PatientConsumListJsonVO::createShared();
		vo->code = 9995;
		vo->message = "patientId is required";
		return vo;
	}
	if (encounterId.empty()) {
		auto vo = PatientConsumListJsonVO::createShared();
		vo->code = 9995;
		vo->message = "encounterId is required";
		return vo;
	}

	// --- Exception handling ---
	try {
		PatientConsumService service;
		auto dtos = service.queryDeviceRequests(patientId, encounterId, consumableRoom);

		auto vo = PatientConsumListJsonVO::createShared();
		vo->success(dtos);

		return vo;
	} catch (const std::exception& ex) {
		auto vo = PatientConsumListJsonVO::createShared();
		vo->code = 9994;
		vo->message = ex.what();
		return vo;
	} catch (...) {
		auto vo = PatientConsumListJsonVO::createShared();
		vo->code = 9999;
		vo->message = "unknown server error";
		return vo;
	}
}
