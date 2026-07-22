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
// 分页DTO头文件
#include "domain/dto/pharm/PharmPageDTO.h"
// 公共库工具类直接引用文件名
#include "SimpleDateTimeFormat.h"

PharmPageJsonVO::Wrapper PharmService::getPage(const PharmQuery::Wrapper& query)
{
    auto result = PharmPageJsonVO::createShared();
    PharmDAO dao;

    // 查询总条数
    uint64_t total = dao.count(query);
    if (total <= 0)
    {
        // 构造空分页DTO，显式创建空列表，避免初始化列表歧义
        auto pageDto = PharmPageDTO::createShared();
        pageDto->rows = oatpp::List<PharmDTO::Wrapper>::createShared();
        pageDto->pageIndex = query->pageIndex.getValue(1);
        pageDto->pageSize = query->pageSize.getValue(10);
        // 用原生变量中转0，消除运算符重载歧义
        uint64_t zeroVal = 0;
        pageDto->total = zeroVal;
        result->fail(ZH_WORDS_GETTER("pharm.result.empty"));
        return result;
    }

    // 分页查询数据
    std::list<PtrPharmDO> dataList = dao.selectPage(query);

    // DO 转 DTO，组装数据列表
    auto rows = oatpp::List<PharmDTO::Wrapper>::createShared();
    for (const auto& item : dataList)
    {
        auto dto = PharmDTO::createShared();
        dto->id = item->getId();
        dto->name = item->getName();
        dto->type = item->getType();
        dto->status = item->getStatus();
        dto->sort = item->getSort();
        dto->remark = item->getRemark();
        dto->create_by = item->getCreate_By();
        dto->update_by = item->getUpdate_By();
        dto->create_time = item->getCreate_Time();
        dto->update_time = item->getUpdate_Time();
        rows->push_back(dto);
    }

    // 构造完整分页DTO
    auto pageDto = PharmPageDTO::createShared();
    pageDto->rows = rows;
    pageDto->pageIndex = query->pageIndex.getValue(1);
    pageDto->pageSize = query->pageSize.getValue(10);
    pageDto->total = total;
    result->success(pageDto);
    return result;
}

StringJsonVO::Wrapper PharmService::add(const PharmAddDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    // DTO 转 DO
    PharmDO data;
    data.setName(dto->name.getValue(""));
    data.setType(dto->type.getValue(""));
    data.setSort(dto->sort.getValue(0));
    data.setRemark(dto->remark.getValue(""));

    // getPayload() 返回指针，用 -> 访问成员
    std::string username = dto->getPayload()->getUsername();
    data.setCreate_By(username);
    data.setUpdate_By(username);
    std::string now = SimpleDateTimeFormat::format();
    data.setCreate_Time(now);
    data.setUpdate_Time(now);

    // 新增默认启用状态
    data.setStatus(1);

    // 执行新增
    PharmDAO dao;
    int rows = dao.insert(data);
    if (rows > 0)
    {
        result->success(ZH_WORDS_GETTER("pharm.result.add_success"));
    }
    else
    {
        result->fail(ZH_WORDS_GETTER("pharm.result.add_fail"));
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

    // getPayload() 返回引用，用 . 访问成员
    std::string username = dto->getPayload().getUsername();
    data.setUpdate_By(username);
    data.setUpdate_Time(SimpleDateTimeFormat::format());

    PharmDAO dao;
    if (dao.update(data) == 1)
    {
        result->success(ZH_WORDS_GETTER("pharm.result.modify_success"));
    }
    else
    {
        result->fail(ZH_WORDS_GETTER("pharm.result.modify_fail"));
    }
    return result;
}

StringJsonVO::Wrapper PharmService::remove(uint64_t id)
{
    auto result = StringJsonVO::createShared();
    PharmDAO dao;
    if (dao.remove(id) == 1)
    {
        result->success(ZH_WORDS_GETTER("pharm.result.remove_success"));
    }
    else
    {
        result->fail(ZH_WORDS_GETTER("pharm.result.remove_fail"));
    }
    return result;
}

StringJsonVO::Wrapper PharmService::updateStatus(uint64_t id, int status)
{
    auto result = StringJsonVO::createShared();
    PharmDAO dao;
    if (dao.updateStatus(id, status) == 1)
    {
        result->success(ZH_WORDS_GETTER("pharm.result.status_success"));
    }
    else
    {
        result->fail(ZH_WORDS_GETTER("pharm.result.status_fail"));
    }
    return result;
}