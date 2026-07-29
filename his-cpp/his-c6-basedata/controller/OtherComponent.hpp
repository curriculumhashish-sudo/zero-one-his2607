#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/12/09 20:32:13

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
#ifndef _OTHERCOMMPONENT_H_
#define _OTHERCOMMPONENT_H_
#include "AbstractComponentReg.h"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

/**
 * Registration of other Oatpp components.
 * External definitions can be added here for future component extensions.
 */
class OtherComponent : public AbstractComponentReg
{
	// 注册药品声明式服务调用组件。
	OATPP_CREATE_COMPONENT(
		std::shared_ptr<oatpp::web::client::HttpRequestExecutor>,
		drugApiExecutor)("drug-api", [] {
			auto provider = oatpp::network::tcp::client::ConnectionProvider::createShared(
				{"192.168.28.132", 10000});
			return oatpp::web::client::HttpRequestExecutor::createShared(provider);
		}());
};

#endif // _OTHERCOMMPONENT_H_
