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
#include "PharmController.h"
#include "../../service/pharm/PharmService.h"

// 分页查询库房药房列表
PharmPageJsonVO::Wrapper PharmController::execGetPharmList(const PharmQuery::Wrapper& query)
{
    PharmService service;
    return service.getPage(query);
}

// 新增库房药房
StringJsonVO::Wrapper PharmController::execAddPharm(const PharmAddDTO::Wrapper& dto)
{
    // 前端参数校验
    std::string err = dto->validate();
    if (!err.empty()) {
        auto result = StringJsonVO::createShared();
        result->fail(oatpp::String(err.c_str()));
        return result;
    }

    PharmService service;
    return service.add(dto);
}

// 修改库房药房
StringJsonVO::Wrapper PharmController::execModifyPharm(const PharmModifyDTO::Wrapper& dto)
{
    PharmService service;
    return service.modify(dto);
}

// 删除库房药房
StringJsonVO::Wrapper PharmController::execRemovePharm(const String& id)
{
    // 字符串id转无符号整型
    //uint64_t idNum = std::stoull(id->std_str());
    //uint64_t idNum = std::stoull(id.std_str());
    uint64_t idNum = std::stoull(id->c_str());
    PharmService service;
    return service.remove(idNum);
}

// 切换库房药房状态
StringJsonVO::Wrapper PharmController::execUpdatePharmStatus(uint64_t id, int status)
{
    PharmService service;
    return service.updateStatus(id, status);
}