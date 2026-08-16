#include "stdafx.h"
#include "GetSettleHistoryController.h"

#include <stdexcept>
#include <string>

#include "service/settle/GetSettleHistoryService.h"
#include "service/settle/SettlementValidator.h"

GetSettleHistoryJsonVO::Wrapper GetSettleHistoryController::execGetSettleHistory(const GetSettleHistoryQuery::Wrapper &query) {
	auto vo = GetSettleHistoryJsonVO::createShared();
	const std::string validationMessage =
		SettlementValidator::validateSettleHistoryQuery(query);
	if (!validationMessage.empty()) {
		vo->init(nullptr, ResultStatus(validationMessage, 9995));
		return vo;
	}

	try {
		vo->success(GetSettleHistoryService().getSettleHistory(query));
	}
	catch (const std::invalid_argument &exception) {
		vo->init(nullptr, ResultStatus(exception.what(), 9995));
	}
	catch (const std::exception &) {
		vo->init(nullptr, ResultStatus(ZH_WORDS_GETTER("ovo.charging.settle.get_settle_history_failed"), 9994));
	}
	return vo;
}
