/*
 Copyright Zero One Star. All rights reserved.

 @Author: your name
 @Date: 2026/7/22

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

PharmPageJsonVO::Wrapper PharmService::getPage(const PharmQuery::Wrapper& query)
{
    auto result = PharmPageJsonVO::createShared();
    PharmDAO dao;

    // ��ѯ������
    uint64_t total = dao.count(query);

    // �ȹ����ҳDTO��������Ҳ���������ṹ
    auto pageDto = PharmPageDTO::createShared();
    pageDto->pageIndex = query->pageIndex.getValue(1);
    pageDto->pageSize = query->pageSize.getValue(10);
    pageDto->total = total;
    pageDto->rows = oatpp::List<PharmDTO::Wrapper>::createShared();

    // ������ʱ�ٲ�ѯ�б�����DOתDTO
    if (total > 0)
    {
        std::list<PtrPharmDO> dataList = dao.selectPage(query);
        auto rows = oatpp::List<PharmDTO::Wrapper>::createShared();
        for (const auto& item : dataList)
        {
            auto dto = PharmDTO::createShared();
            dto->id = item->getId();
            dto->name = oatpp::String(item->getName());
            dto->type = oatpp::String(item->getType());
            dto->status = item->getStatus();
            dto->sort = item->getSort();
            dto->remark = oatpp::String(item->getRemark());
            dto->create_by = oatpp::String(item->getCreate_By());
            dto->update_by = oatpp::String(item->getUpdate_By());
            dto->create_time = oatpp::String(item->getCreate_Time());
            dto->update_time = oatpp::String(item->getUpdate_Time());
            rows->push_back(dto);
        }
        pageDto->rows = rows;
    }

    result->success(pageDto);
    return result;
}

PharmNameListJsonVO::Wrapper PharmService::nameList(const oatpp::String& type)
{
    auto result = PharmNameListJsonVO::createShared();
    PharmDAO dao;

    std::string typeStr;
    if (type)
    {
        typeStr = type.getValue("");
    }

    std::list<PtrPharmDO> dataList = dao.nameList(typeStr);
    auto rows = oatpp::List<PharmNameDTO::Wrapper>::createShared();
    for (const auto& item : dataList)
    {
        auto dto = PharmNameDTO::createShared();
        dto->id = item->getId();
        dto->name = oatpp::String(item->getName());
        rows->push_back(dto);
    }

    result->success(rows);
    return result;
}

StringJsonVO::Wrapper PharmService::add(const PharmAddDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    // DTO ת DO
    PharmDO data;
    data.setName(dto->name.getValue(""));
    data.setType(dto->type.getValue(""));
    data.setSort(dto->sort.getValue(0));
    data.setRemark(dto->remark.getValue(""));

    // ��ȡ�����ˣ�payload��������ʵ�û�������������Ĭ��admin����
    std::string username = "admin";
    if (dto->getPayload())
    {
        username = dto->getPayload()->getUsername();
    }
    data.setCreate_By(username);
    data.setUpdate_By(username);
    std::string now = SimpleDateTimeFormat::format();
    data.setCreate_Time(now);
    data.setUpdate_Time(now);

    // ����Ĭ������״̬
    data.setStatus(1);

    // ִ�����ݿ����
    PharmDAO dao;
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

    PharmDO data;
    data.setId(dto->id.getValue(0));
    data.setName(dto->name.getValue(""));
    data.setType(dto->type.getValue(""));
    data.setSort(dto->sort.getValue(0));
    data.setRemark(dto->remark.getValue(""));

    // ��ȡ�����ˣ�payload��������ʵ�û�������������Ĭ��admin����
    std::string username = "admin";
    if (dto->getPayload())
    {
        username = dto->getPayload()->getUsername();
    }
    data.setUpdate_By(username);
    data.setUpdate_Time(SimpleDateTimeFormat::format());

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

StringJsonVO::Wrapper PharmService::remove(uint64_t id)
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

StringJsonVO::Wrapper PharmService::updateStatus(uint64_t id, int status)
{
    auto result = StringJsonVO::createShared();
    PharmDAO dao;
    if (dao.updateStatus(id, status) == 1)
    {
        result->success(oatpp::String(ZH_WORDS_GETTER("pharm.result.status_success")));
    }
    else
    {
        result->fail(oatpp::String(ZH_WORDS_GETTER("pharm.result.status_fail")));
    }
    return result;
}