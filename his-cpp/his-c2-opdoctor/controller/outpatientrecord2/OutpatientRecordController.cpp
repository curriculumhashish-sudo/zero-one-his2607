#include "stdafx.h"
#include "OutpatientRecordController.h"
#include "../../service/outpatientrecord2/OutpatientRecordService.h"

// 列表查询（保持不变）
OutpatientRecordPageJsonVO::Wrapper
OutpatientRecordController::execQueryRecordList(const OutpatientRecordQuery::Wrapper& query, const PayloadDTO& payload) {
    auto jvo = OutpatientRecordPageJsonVO::createShared();
    try {
        OutpatientRecordService service;
        auto pageDto = service.listAll(query, payload);
        jvo->code = 0;
        jvo->message = "success";
        jvo->data = pageDto;
    }
    catch (const std::exception& e) {
        jvo->code = 9994;
        jvo->message = e.what();
    }
    return jvo;
}
// 详情接口（直接返回 DTO）
OutpatientRecordDetailJsonVO::Wrapper
OutpatientRecordController::execGetRecordDetail(const String& encounterId, const PayloadDTO& payload) {
    auto resp = OutpatientRecordDetailJsonVO::createShared();
    try {
        if (!encounterId || encounterId->empty())
            throw std::runtime_error("encounterId is required");
        OutpatientRecordService service;
        resp->code = 0;
        resp->message = "success";
        resp->data = service.getDetail(encounterId->c_str(), payload);
    }
    catch (const std::exception& e) {
        resp->code = 9994;
        resp->message = e.what();
    }
    return resp;
}