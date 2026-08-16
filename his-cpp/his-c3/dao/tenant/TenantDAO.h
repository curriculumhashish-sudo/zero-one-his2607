#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: nick
 @Date: 2026/07/20 12:00:00

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
#ifndef _TENANTDAO_H_
#define _TENANTDAO_H_
#include "BaseDAO.h"
#include "../../domain/do/tenant/TenantDO.h"

/**
 *租户数据访问对象类
 */
class TenantDAO : public BaseDAO
{
public:
	//新增和修改继承自 BaseDAO,只定义软删除
	//按 ID 软删除租户，设置 is_deleted=1、deleted_at、deleted_by，并在 code 后拼接 '#deleted_{id}' 来释放唯一约束
	int softDelete(const std::string& id, const std::string& deletedBy);
};
#endif // !_TENANTDAO_H_
