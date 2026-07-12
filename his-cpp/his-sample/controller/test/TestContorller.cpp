/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/12 21:35:59

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
#include "TestContorller.h"

StringJsonVO::Wrapper TestController::executeQueryTest(const PageQuery::Wrapper& query)
{
	std::cout << "executeQueryTest" << std::endl;
	std::cout << "query.getPageSize() = " << query->pageSize.getValue({}) << std::endl;
	std::cout << "query.getPageIndex() = " << query->pageIndex.getValue({}) << std::endl;
	// 模拟响应结果
	auto vo = StringJsonVO::createShared();
	vo->success(String(ZH_WORDS_GETTER("test.get.result")));
	return vo;
}
