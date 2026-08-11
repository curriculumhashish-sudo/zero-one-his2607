#include "stdafx.h"
#include "ReceptionPatientController.h"
#include "service/cycleCtrl1/ReceptionPatientService.h"
#include "RedisClient.h"


RecePatientJsonVO::Wrapper RecePatientController::execQueryRece(const RecePatientQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = RecePatientJsonVO::createShared();

    // 参数校验
    if (!query || !query->date)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // 从payload读取practitionerId
    std::string practitionerId = payload.getPractitionerId();

    // 创建 Redis 客户端以获取 tenantId
// 填入表格真实RedisIP、端口6380、实际密码
    RedisClient re("8.163.102.33", 6380, "Lin@@2830564901zero-one-his");
    std::string tenantId = re.execute<std::string>([](Redis* r) {
        auto res = r->hget("his:login:context:u001", "tenantId");
        return res.value();
        });

	//如果practitionerid和tenantId为空，则使用默认值
    if (practitionerId.empty() || tenantId.empty())
    {
        practitionerId = "";
        tenantId = "tenant_001";
    }

    auto dto = ReceptionPatientService().getRecePatient(query, tenantId, practitionerId);

    // 包装为成功响应
    jvo->success(dto);
    return jvo;
}

RecePatientJsonVO::Wrapper RecePatientController::execAcceptRece(const RecePatientAcceptQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = RecePatientJsonVO::createShared();

    if (!query || !query->encounterId)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    // practitionerId 从 payload
    std::string practitionerId = payload.getPractitionerId();

    // tenantId 仍沿用现有获取方式（与其他 controller 一致）
    RedisClient re("8.163.102.33", 6380, "Lin@@2830564901zero-one-his");
    std::string tenantId = re.execute<std::string>([](Redis* r) {
        auto res = r->hget("his:login:context:u001", "tenantId");
        return res.value();
        });

    if (practitionerId.empty() || tenantId.empty())
    {
        practitionerId = "";
        tenantId = "tenant_001";
    }

    auto dto = ReceptionPatientService().acceptPatientByEncounterId(query->encounterId, tenantId, practitionerId);

    jvo->success(dto);
    return jvo;
}