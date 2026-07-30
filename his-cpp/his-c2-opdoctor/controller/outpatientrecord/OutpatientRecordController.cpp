#include "stdafx.h"
#include "OutpatientRecordController.h"
#include "../../service/outpatientrecord/OutpatientRecordService.h"

OutpatientRecordPageJsonVO::Wrapper OutpatientRecordController::execQueryRecord(const OutpatientRecordQuery::Wrapper& query)
{
    auto jvo = OutpatientRecordPageJsonVO::createShared();
    OutpatientRecordService service;
    auto pageDto = service.listAll(query);
    jvo->success(pageDto);
    return jvo;
}