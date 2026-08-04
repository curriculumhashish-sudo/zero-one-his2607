#include "stdafx.h"
#include "Outpatientrecord3Controller.h"

StringJsonVO::Wrapper
outpatientrecord33Controller::execSaveoutpatientrecord3(
	const outpatientrecord3SaveDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	return service.save(dto, payload);
}
