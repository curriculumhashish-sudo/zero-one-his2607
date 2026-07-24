/*
 Copyright Zero One Star. All rights reserved.

 @Author: xinpingqihe
 @Date: 2026/07/20 00:00:00

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
#include "CostDetailController.h"
#include "service/chargedetails/CostDetailService.h"
#include <iostream>

CostDetailPageJsonVO::Wrapper CostDetailController::execQueryAll(const CostDetailQuery::Wrapper& query) {
	auto vo = CostDetailPageJsonVO::createShared();
	try {
		CostDetailService service;
		auto pageDto = service.queryAll(query);
		vo->success(pageDto);
	}
	catch (const std::exception& e) {
		std::cerr << "execQueryAll error: " << e.what() << std::endl;
		vo->fail({});
	}
	return vo;
}

CostDetailJsonVO::Wrapper CostDetailController::execQueryOne(const CostSummaryQuery::Wrapper& query) {
	auto vo = CostDetailJsonVO::createShared();
	try {
		CostDetailService service;
		auto dto = service.queryOne(query->id);
		if (dto->id) {
			vo->success(dto);
		}
		else {
			vo->fail({});
		}
	}
	catch (const std::exception& e) {
		std::cerr << "execQueryOne error: " << e.what() << std::endl;
		vo->fail({});
	}
	return vo;
}

CostSummaryListJsonVO::Wrapper CostDetailController::execSummaryByEncounter(const CostSummaryQuery::Wrapper& query) {
	auto vo = CostSummaryListJsonVO::createShared();
	try {
		CostDetailService service;
		auto list = service.summaryByEncounter(query->encounterId);
		vo->success(list);
	}
	catch (const std::exception& e) {
		std::cerr << "execSummaryByEncounter error: " << e.what() << std::endl;
		vo->fail({});
	}
	return vo;
}

CostSummaryListJsonVO::Wrapper CostDetailController::execSummaryByDate(const CostSummaryQuery::Wrapper& query) {
	auto vo = CostSummaryListJsonVO::createShared();
	try {
		CostDetailService service;
		auto list = service.summaryByDate(query->startDate, query->endDate, query->encounterId);
		vo->success(list);
	}
	catch (const std::exception& e) {
		std::cerr << "execSummaryByDate error: " << e.what() << std::endl;
		vo->fail({});
	}
	return vo;
}
