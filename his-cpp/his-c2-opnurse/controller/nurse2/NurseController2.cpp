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
#include "NurseController2.h"
#include "../../service/nurse2/nurseService2.h"

StringJsonVO::Wrapper NurseController2::execOperateNurse(const NurseOperateDTO2::Wrapper& dto, const PayloadDTO& payload)
{
	// Create response object
	auto jvo = StringJsonVO::createShared();
	// Validate required field
	if (!dto->name)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	// Execute nurse operation via service
	if (NurseService2().NurseOperate2(dto, payload.getUsername()))
	{
		jvo->success(dto->name);
	}
	else
	{
		jvo->fail(dto->name);
	}
	return jvo;
}

StringJsonVO::Wrapper NurseController2::execCancelNurse(const NurseCancelDTO2::Wrapper& dto, const PayloadDTO& payload)
{
	// Create response object
	auto jvo = StringJsonVO::createShared();
	// Validate required field
	if (!dto->name)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	// Cancel nurse execution via service
	if (NurseService2().NurseCancel2(dto, payload.getUsername()))
	{
		jvo->success(dto->name);
	}
	else
	{
		jvo->fail(dto->name);
	}
	return jvo;
}
