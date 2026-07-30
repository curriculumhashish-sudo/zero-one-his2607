/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/07/24

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
#include "nurseService2.h"
#include "domain/do/nurse2/nurseDO2.h"
#include "dao/nurse2/NurseDAO2.h"

bool NurseService2::NurseOperate2(const NurseOperateDTO2::Wrapper& dto, const std::string& username)
{
	// 转换 DTO to DO
	NurseDO2 data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Name, name, State, state,
		Content, content, Amount, amount, Operate, operate);
	// 从身份认证载荷（payload）中赋值执行人信息（覆盖DTO里原本的人员字段）
	data.setPerson(username);

	// 根据业务编号（排队号）匹配对应记录，并执行更新操作
	NurseDAO2 dao;
	std::string where = "`bus_no`='" + dto->name.getValue("") + "'";
	return dao.update(data, where) == 1;
}

bool NurseService2::NurseCancel2(const NurseCancelDTO2::Wrapper& dto, const std::string& username)
{
	//  DTO to DO
	NurseDO2 data;
	ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, Name, name, State, state,
		Content, content, Amount, amount, Operate, operate);
	// 从身份认证载荷（payload）中赋值执行人信息（覆盖DTO里原本的人员字段）
	data.setPerson(username);

	// 根据排队业务编号（bus_no）匹配目标记录并执行数据更新
	NurseDAO2 dao;
	std::string where = "`bus_no`='" + dto->name.getValue("") + "'";
	return dao.update(data, where) == 1;
}
