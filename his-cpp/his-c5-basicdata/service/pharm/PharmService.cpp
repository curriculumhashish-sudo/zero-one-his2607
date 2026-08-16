/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/01 10:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "PharmService.h"
#include "../../dao/pharm/PharmDAO.h"
#include "domain/dto/pharm/PharmDTO.h"
#include "domain/dto/pharm/PharmPageDTO.h"
#include "SimpleDateTimeFormat.h"
#include "id/SnowFlake.h"

PharmPageJsonVO::Wrapper PharmService::getPage(const PharmQuery::Wrapper& query)
{
    auto result = PharmPageJsonVO::createShared();
    PharmDAO dao;

    // Query total count
    uint64_t total = dao.count(query);

    // Build page DTO envelope
    auto pageDto = PharmPageDTO::createShared();
    pageDto->pageIndex = query->pageIndex.getValue(1);
    pageDto->pageSize = query->pageSize.getValue(10);
    pageDto->total = total;
    pageDto->rows = oatpp::List<PharmDTO::Wrapper>::createShared();

    // Query list and map DO to DTO
    if (total > 0)
    {
        std::list<PtrPharmDO> dataList = dao.selectPage(query);
        auto rows = oatpp::List<PharmDTO::Wrapper>::createShared();
        for (const auto& item : dataList)
        {
            auto dto = PharmDTO::createShared();
            dto->id = oatpp::String(item->getId());
            dto->name = oatpp::String(item->getName());
            dto->formEnum = oatpp::String(item->getFormEnum());
            dto->statusEnum = oatpp::String(item->getStatusEnum());
            dto->busNo = oatpp::String(item->getBusNo());
            dto->organizationId = oatpp::String(item->getOrganizationId());
            dto->displayOrder = item->getDisplayOrder();
            dto->pyStr = oatpp::String(item->getPyStr());
            dto->wbStr = oatpp::String(item->getWbStr());
            dto->parentId = oatpp::String(item->getParentId());
            dto->createdBy = oatpp::String(item->getCreatedBy());
            dto->updatedBy = oatpp::String(item->getUpdatedBy());
            dto->createdAt = oatpp::String(item->getCreatedAt());
            dto->updatedAt = oatpp::String(item->getUpdatedAt());
            rows->push_back(dto);
        }
        pageDto->rows = rows;
    }

    result->success(pageDto);
    return result;
}

PharmNameListJsonVO::Wrapper PharmService::nameList(const oatpp::String& formEnum)
{
    auto result = PharmNameListJsonVO::createShared();
    PharmDAO dao;

    std::string formEnumStr;
    if (formEnum)
    {
        formEnumStr = formEnum.getValue("");
    }

    std::list<PtrPharmDO> dataList = dao.nameList(formEnumStr);
    auto rows = oatpp::List<PharmNameDTO::Wrapper>::createShared();
    for (const auto& item : dataList)
    {
        auto dto = PharmNameDTO::createShared();
        dto->id = oatpp::String(item->getId());
        dto->name = oatpp::String(item->getName());
        rows->push_back(dto);
    }

    result->success(rows);
    return result;
}

StringJsonVO::Wrapper PharmService::add(const PharmAddDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    // Generate snowflake ID
    std::string id = std::to_string(SnowFlake::getInstance()->nextId());

    // Map DTO to DO
    PharmDO data;
    data.setId(id);
    data.setName(dto->name.getValue(""));
    data.setFormEnum(dto->formEnum.getValue(""));
    data.setBusNo(dto->busNo.getValue(""));
    data.setOrganizationId(dto->organizationId.getValue(""));
    data.setDisplayOrder(dto->displayOrder.getValue(0));
    data.setPyStr(dto->pyStr.getValue(""));
    data.setWbStr(dto->wbStr.getValue(""));
    data.setParentId(dto->parentId.getValue(""));

    // Set user audit fields from JWT payload (default to "admin")
    std::string username = "admin";
    if (dto->getPayload())
    {
        username = dto->getPayload()->getUsername();
    }
    std::string now = SimpleDateTimeFormat::format();
    data.setCreatedBy(username);
    data.setUpdatedBy(username);
    data.setCreatedAt(now);
    data.setUpdatedAt(now);

    // Set default status
    data.setStatusEnum("ACTIVE");
    data.setModeEnum("PHARM");

    // Check for duplicate name before insert
    PharmDAO dao;
    if (dao.existsName(data.getName()))
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.name_duplicate")));
        return result;
    }

    // Execute database insert
    int rows = dao.insert(data);
    if (rows > 0)
    {
        result->success(oatpp::String(ZH_WORDS_GETTER("pharm.result.add_success")));
    }
    else
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.add_fail")));
    }
    return result;
}

StringJsonVO::Wrapper PharmService::modify(const PharmModifyDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    // Map DTO to DO
    PharmDO data;
    data.setId(dto->id.getValue(""));
    data.setName(dto->name.getValue(""));
    data.setFormEnum(dto->formEnum.getValue(""));
    data.setBusNo(dto->busNo.getValue(""));
    data.setOrganizationId(dto->organizationId.getValue(""));
    data.setDisplayOrder(dto->displayOrder.getValue(0));
    data.setPyStr(dto->pyStr.getValue(""));
    data.setWbStr(dto->wbStr.getValue(""));
    data.setParentId(dto->parentId.getValue(""));

    // Set user audit fields from JWT payload
    std::string username = "admin";
    if (dto->getPayload())
    {
        username = dto->getPayload()->getUsername();
    }
    data.setUpdatedBy(username);
    data.setUpdatedAt(SimpleDateTimeFormat::format());

    PharmDAO dao;
    if (dao.update(data) == 1)
    {
        result->success(oatpp::String(ZH_WORDS_GETTER("pharm.result.modify_success")));
    }
    else
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.modify_fail")));
    }
    return result;
}

StringJsonVO::Wrapper PharmService::remove(const std::string& id)
{
    auto result = StringJsonVO::createShared();
    PharmDAO dao;
    if (dao.remove(id) == 1)
    {
        result->success(oatpp::String(ZH_WORDS_GETTER("pharm.result.remove_success")));
    }
    else
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.remove_fail")));
    }
    return result;
}

StringJsonVO::Wrapper PharmService::updateStatus(const std::string& id, const std::string& statusEnum)
{
    auto result = StringJsonVO::createShared();
    PharmDAO dao;
    if (dao.updateStatus(id, statusEnum) == 1)
    {
        result->success(oatpp::String(ZH_WORDS_GETTER("pharm.result.status_success")));
    }
    else
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.status_fail")));
    }
    return result;
}
