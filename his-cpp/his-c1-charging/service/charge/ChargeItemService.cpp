#include "stdafx.h"
#include "ChargeItemService.h"
#include "dao/charge/ChargeItemDAO.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

#include <sstream>
#include <iomanip>

GetChargeItemDTO::Wrapper ChargeItemService::listAll(const GetChargeItemsQuery::Wrapper& query)
{
    auto dto = GetChargeItemDTO::createShared();
    dto->encounterId = query->encounterId;

    auto items = oatpp::Vector<oatpp::Object<GetChargeItemRowDTO>>::createShared();

    if (query->encounterId && !query->encounterId->empty())
    {
        ChargeItemDAO dao;
        auto doList = dao.selectByEncounterId(query->encounterId);

        for (auto& item : doList)
        {
            auto row = GetChargeItemRowDTO::createShared();
            row->chargeItemId = item.getChargeItemId();
            row->serviceTable = item.getServiceTable();
            row->serviceId = item.getServiceId();
            row->itemName = item.getItemName();
            row->busNo = item.getBusNo();

            try { row->unitPrice = std::stod(item.getUnitPrice()); }
            catch (...) { row->unitPrice = 0.0; }
            try { row->quantityValue = std::stod(item.getQuantityValue()); }
            catch (...) { row->quantityValue = 0.0; }

            row->quantityUnit = item.getQuantityUnit();

            // 计算数量展示文本
            std::ostringstream qd;
            qd << std::fixed << std::setprecision(0) << row->quantityValue;
            row->quantityDisplay = qd.str();

            row->chargeCategoryCode = item.getChargeCategoryCode();
            row->chargeCategoryDisplay = ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.chargeCategoryDisplay.value");
            row->statusCode = item.getStatusCode();
            row->statusText = ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.statusText.value");

            try { row->totalPrice = std::stod(item.getTotalPrice()); }
            catch (...) { row->totalPrice = 0.0; }

            row->cashierUserId = item.getCashierUserId();
            row->cashierName = item.getCashierName();
            row->discountRateDisplay = ZH_WORDS_GETTER("lhfc.charging.charge.GetChargeItemRowDTO.discountRateDisplay.value");
            row->accountId = item.getAccountId();
            row->contractName = item.getContractName();
            row->contractId = item.getContractId();
            row->feeNatureDisplay = item.getContractName();
            row->medTypeCode = item.getMedTypeCode();
            row->ybNo = item.getYbNo();
            row->medicalInsuranceCode = item.getYbNo();

            items->push_back(row);
        }
    }

    dto->items = items;
    return dto;
}
