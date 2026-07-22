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

// 分页查询库房药房列表（占位实现，后续接入Service后替换）
PharmPageJsonVO::Wrapper PharmController::execGetPharmList(const PharmQuery::Wrapper& query)
{
    return {};
}

// 新增库房药房（占位实现）
StringJsonVO::Wrapper PharmController::execAddPharm(const PharmAddDTO::Wrapper& dto)
{
    return {};
}

// 修改库房药房（占位实现）
StringJsonVO::Wrapper PharmController::execModifyPharm(const PharmModifyDTO::Wrapper& dto)
{
    return {};
}

// 删除库房药房（占位实现）
StringJsonVO::Wrapper PharmController::execRemovePharm(const String& id)
{
    return {};
}

// 切换库房药房状态（占位实现）
// 修复：参数类型和头文件声明、宏调用完全对齐
StringJsonVO::Wrapper PharmController::execUpdatePharmStatus(uint64_t id, int status)
{
    return {};
}