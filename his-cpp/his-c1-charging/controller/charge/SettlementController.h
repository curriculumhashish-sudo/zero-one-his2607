#pragma once
#ifndef _SETTLEMENT_CONTROLLER_
#define _SETTLEMENT_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/charge/PreSettleChargeDTO.h"
#include "domain/vo/charge/SettleChargeVO.h"
#include "domain/dto/charge/ConfirmSettleChargeDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.charge.tag")
/*
   结算控制器
   负责预结算（生成草稿锁定收费项）和正式结算（落账收费）
*/
class SettlementController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(SettlementController);
public:
    // 预结算——生成草稿，锁定收费项
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.PreSettleCharge.summary"),
        preSettleCharge,
        PreSettleChargeVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charging/charge/pre-settle-charge", preSettleCharge,
        BODY_DTO(PreSettleChargeRequestDTO::Wrapper, dto), execPreSettleCharge(dto)
    );

    // 正式结算——落账收费
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmSettleCharge.summary"),
        confirmSettleCharge,
        ConfirmSettleChargeVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST, "/charging/charge/confirm-settle-charge", confirmSettleCharge,
        BODY_DTO(ConfirmSettleChargeRequestDTO::Wrapper, dto),
        execConfirmSettleCharge(dto, authObject->getPayload())
    );

private:
    // 预结算
    PreSettleChargeVO::Wrapper execPreSettleCharge(const PreSettleChargeRequestDTO::Wrapper& dto);
    // 正式结算
    ConfirmSettleChargeVO::Wrapper execConfirmSettleCharge(
        const ConfirmSettleChargeRequestDTO::Wrapper& dto,
        const PayloadDTO& payload);
};
#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_SETTLEMENT_CONTROLLER_
