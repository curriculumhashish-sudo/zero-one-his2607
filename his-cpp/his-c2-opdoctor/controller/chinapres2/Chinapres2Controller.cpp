#include "stdafx.h"
#include "Chinapres2Controller.h"
#include "../../dao/chinapres2/Chinapres2DAO.h"
#include <chrono>
#include "../../service/chinapres2/Chinapres2Service.h"

Chinapres2AddRespJsonVO::Wrapper
Chinapres2Controller::execAddChinapre(const Chinapres2AddDTO::Wrapper& dto, const PayloadDTO& payload) {
    auto resp = Chinapres2AddRespJsonVO::createShared();
    try {
        if (!dto) throw std::runtime_error("dto is null");
        std::string err = dto->validate();
        if (!err.empty()) throw std::runtime_error(err);
        Chinapres2Service service;
        resp->code = 0;
        resp->message = "success";
        resp->data = service.saveData(dto, payload);
    }
    catch (const std::exception& e) {
        resp->code = 9994;
        resp->message = e.what();
    }
    return resp;
}

Chinapres2AddRespJsonVO::Wrapper
Chinapres2Controller::execUpdateChinapre(const Chinapres2UpdateDTO::Wrapper& dto, const PayloadDTO& payload) {
    auto resp = Chinapres2AddRespJsonVO::createShared();
    try {
        if (!dto) throw std::runtime_error("dto is null");
        Chinapres2Service service;
        resp->code = 0;
        resp->message = "success";
        resp->data = service.updateData(dto, payload);
    }
    catch (const std::exception& e) {
        resp->code = 9994;
        resp->message = e.what();
    }
    return resp;
}

Chinapres2RemoveRespJsonVO::Wrapper
Chinapres2Controller::execRemoveChinapreEnhanced(const Chinapres2RemoveDTO::Wrapper& dto, const PayloadDTO& payload) {
    auto resp = Chinapres2RemoveRespJsonVO::createShared();
    try {
        if (!dto) throw std::runtime_error("dto is null");
        if (!dto->id || dto->id->empty() || !dto->encounterId || dto->encounterId->empty() || !dto->version)
            throw std::runtime_error("id, encounterId, version are required");
        Chinapres2Service service;
        resp->code = 0;
        resp->message = "success";
        resp->data = service.removeDataEnhanced(dto->id->c_str(), dto->encounterId->c_str(), *dto->version, payload);
    }
    catch (const std::exception& e) {
        resp->code = 9994;
        resp->message = e.what();
    }
    return resp;
}// ========== 2. Query Prescription List ==========
/*
Chinapres2DetailPageJsonVO::Wrapper Chinapres2Controller::execQueryChinapre(const Chinapres2Query::Wrapper& query) {
    auto jvo = Chinapres2DetailPageJsonVO::createShared();

    Chinapres2Service service;
    auto pageDto = service.listAll(query);

    jvo->success(pageDto);
    return jvo;
}
*/

