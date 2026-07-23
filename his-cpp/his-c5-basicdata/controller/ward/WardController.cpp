#include "stdafx.h"
#include "WardController.h"

WardRoomJsonVO::Wrapper WardController:: execQueryRoomById(const String& id){
	return WardRoomJsonVO::createShared();
}
WardBedJsonVO::Wrapper WardController:: execQueryBedById(const String & id) {
	return WardBedJsonVO::createShared();
}
