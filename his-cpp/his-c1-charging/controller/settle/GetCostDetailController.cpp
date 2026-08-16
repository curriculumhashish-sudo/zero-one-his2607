/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
 @Date: 2026/07/20

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
#include "GetCostDetailController.h"

#include <stdexcept>
#include <string>

#include "service/settle/GetCostDetailService.h"
#include "service/settle/SettlementValidator.h"

GetCostDetailJsonVO::Wrapper GetCostDetailController::execGetCostDetail(const GetCostDetailQuery::Wrapper &query) {
	auto vo = GetCostDetailJsonVO::createShared();
	const std::string validationMessage =
		SettlementValidator::validateCostDetailQuery(query);
	if (!validationMessage.empty()) {
		vo->init(nullptr, ResultStatus(validationMessage, 9995));
		return vo;
	}

	try {
		vo->success(GetCostDetailService().getCostDetail(query));
	}
	catch (const std::invalid_argument &exception) {
		vo->init(nullptr, ResultStatus(exception.what(), 9995));
	}
	catch (const std::exception &) {
		vo->init(nullptr, ResultStatus(ZH_WORDS_GETTER("ovo.charging.settle.get_cost_detail_failed"), 9994));
	}
	return vo;
}
