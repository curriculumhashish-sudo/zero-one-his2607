#include "stdafx.h"
#include "SettingItemsController.h"
#include "../../service/settingitems/SettingItemsService.h"

StringJsonVO::Wrapper SettingItemsController::execAdd(const SettingAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = StringJsonVO::createShared();
	// Attach auth payload so Service can read the current user
	dto->setPayload(&payload);

	std::string id = SettingItemsService().addData(dto);
	if (id != "") {
		jvo->success(id);
	} else {
		jvo->fail("");
	}
	return jvo;
}

StringJsonVO::Wrapper SettingItemsController::execModify(const SettingModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = StringJsonVO::createShared();
	// Attach auth payload so Service can read the current user
	dto->setPayload(&payload);

	if (SettingItemsService().modifyData(dto)) {
		jvo->success(dto->id.getValue(""));
	} else {
		jvo->fail("");
	}
	return jvo;
}

Uint64JsonVO::Wrapper SettingItemsController::execRemove(const SettingRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = Uint64JsonVO::createShared();
	auto id = dto->id.getValue("");
	auto deleteby = payload.getUsername();

	if (SettingItemsService().removeData(id, deleteby)) {
		vo->success(UInt64(1));
	} else {
		vo->fail(UInt64());
	}
	return vo;
}
