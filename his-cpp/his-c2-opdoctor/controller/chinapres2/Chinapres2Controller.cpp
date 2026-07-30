#include "stdafx.h"
#include "Chinapres2Controller.h"
#include "../../dao/chinapres2/Chinapres2DAO.h"
#include <chrono>
#include "../../service/chinapres2/Chinapres2Service.h"

// ========== 1. Add Prescription ==========
StringJsonVO::Wrapper Chinapres2Controller::execAddChinapre(const Chinapres2AddDTO::Wrapper& dto, const PayloadDTO& payload) {
    auto jvo = StringJsonVO::createShared();

    // 1. 参数校验
    if (!dto || !dto->details || dto->details->empty()) {
        jvo->fail("dto or details is empty");
        return jvo;
    }

    // 2. 调用 Service 保存数据
    Chinapres2Service service;
    std::string newId = service.saveData(dto, payload);

    if (newId.empty()) {
        jvo->fail("save data failed");
        return jvo;
    }

    jvo->success(newId);
    return jvo;
}

// ========== 2. Query Prescription List ==========
Chinapres2DetailPageJsonVO::Wrapper Chinapres2Controller::execQueryChinapre(const Chinapres2Query::Wrapper& query) {
    auto jvo = Chinapres2DetailPageJsonVO::createShared();

    Chinapres2Service service;
    auto pageDto = service.listAll(query);

    jvo->success(pageDto);
    return jvo;
}


// ========== 3. Delete Prescription（使用 Service） ==========
BooleanJsonVO::Wrapper Chinapres2Controller::execRemoveChinapre(const String& id) {
    auto jvo = BooleanJsonVO::createShared();

    if (!id || id->empty()) {
        jvo->fail("id is empty");
        return jvo;
    }

    Chinapres2Service service;
    bool ok = service.removeData(id.getValue(""));

    if (ok) {
        jvo->success(true);
    }
    else {
        jvo->fail("delete failed");
    }
    return jvo;
}