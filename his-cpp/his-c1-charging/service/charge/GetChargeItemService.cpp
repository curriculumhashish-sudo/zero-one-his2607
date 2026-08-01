#include "stdafx.h"
#include "GetChargeItemService.h"
#include "dao/charge/GetChargeItemDAO.h"
#include"domain/dto/charge/GetChargeItemDTO.h"
#include"domain/query/charge/GetChargeItemsQuery.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

#include <sstream>
#include <iomanip>

GetChargeItemDTO::Wrapper GetChargeItemService::listAll(const GetChargeItemsQuery::Wrapper& query)
{
    auto dto = GetChargeItemDTO::createShared();
    dto->encounterId = query->encounterId;

    auto items = oatpp::Vector<oatpp::Object<GetChargeItemRowDTO>>::createShared();

    if (query->encounterId && !query->encounterId->empty())
    {
        GetChargeItemDAO dao;
        auto doList = dao.selectByEncounterId(query->encounterId);

        for (auto& item : doList)
        {
            auto row = GetChargeItemRowDTO::createShared();

            // 宏转换：String -> String 字段，类型一致直接拷贝
            ZO_STAR_DOMAIN_DO_TO_DTO(row, item,
                chargeItemId, ChargeItemId,
                serviceTable, ServiceTable,
                serviceId, ServiceId,
                itemName, ItemName,
                busNo, BusNo,
                quantityUnit, QuantityUnit,
                chargeCategoryCode, ChargeCategoryCode,
                statusCode, StatusCode,
                cashierUserId, CashierUserId,
                cashierName, CashierName,
                accountId, AccountId,
                contractName, ContractName,
                contractId, ContractId,
                medTypeCode, MedTypeCode,
                ybNo, YbNo
            );

            // String -> Float64 类型转换
            try { row->unitPrice = std::stod(item.getUnitPrice()); }
            catch (...) { row->unitPrice = 0.0; }
            try { row->quantityValue = std::stod(item.getQuantityValue()); }
            catch (...) { row->quantityValue = 0.0; }
            try { row->totalPrice = std::stod(item.getTotalPrice()); }
            catch (...) { row->totalPrice = 0.0; }

            // 计算字段：数量展示文本
            std::ostringstream qd;
            qd << std::fixed << std::setprecision(0) << row->quantityValue;
            row->quantityDisplay = qd.str();

            // 字典翻译/派生字段
            row->chargeCategoryDisplay = ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryDisplay.value");
            if (item.getStatusCode() == "DRAFT")    row->statusText = ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.pending");
            else if (item.getStatusCode() == "SETTLED")  row->statusText = ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.settled");
            else if (item.getStatusCode() == "REFUNDED") row->statusText = ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.refunded");
            else row->statusText = item.getStatusCode();
            row->discountRateDisplay = ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.discountRateDisplay.value");

            row->feeNatureDisplay = item.getContractName();
            row->medicalInsuranceCode = item.getYbNo();

            items->push_back(row);
        }
    }

    dto->items = items;
    return dto;
}
