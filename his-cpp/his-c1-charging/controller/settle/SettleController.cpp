#include "stdafx.h"
#include "SettleController.h"

#include <stdexcept>
#include <string>

#include "service/settle/SettleService.h"
#include "service/settle/SettlementValidator.h"

SettleJsonVO::Wrapper SettleController::execSettle(
	const SettleRequestDTO::Wrapper &dto,
	const PayloadDTO &payload) {
	auto vo = SettleJsonVO::createShared();
	const std::string validationMessage =
		SettlementValidator::validateSettleRequest(dto);
	if (!validationMessage.empty()) {
		vo->init(nullptr, ResultStatus(validationMessage, 9995));
		return vo;
	}

	try {
		vo->success(SettleService().settle(dto, payload));
	}
	catch (const std::invalid_argument &exception) {
		vo->init(nullptr, ResultStatus(exception.what(), 9995));
	}
	catch (const std::logic_error &exception) {
		vo->init(nullptr, ResultStatus(exception.what(), 9999));
	}
	catch (const std::exception &) {
		vo->init(nullptr, ResultStatus(ZH_WORDS_GETTER("ovo.charging.settle.settle_failed"), 9994));
	}
	return vo;
}
