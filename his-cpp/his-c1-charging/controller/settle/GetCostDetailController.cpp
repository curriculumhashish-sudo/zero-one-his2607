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
		const auto result = GetCostDetailService().getCostDetail(query);
		if (!result) {
			vo->init(nullptr, ResultStatus(ZH_WORDS_GETTER("ovo.charging.settle.encounter_not_found"), 9999));
			return vo;
		}
		vo->success(result);
	}
	catch (const std::invalid_argument &exception) {
		vo->init(nullptr, ResultStatus(exception.what(), 9995));
	}
	catch (const std::exception &) {
		vo->init(nullptr, ResultStatus(ZH_WORDS_GETTER("ovo.charging.settle.get_cost_detail_failed"), 9994));
	}
	return vo;
}
