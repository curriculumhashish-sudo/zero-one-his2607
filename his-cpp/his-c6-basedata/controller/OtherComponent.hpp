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
#include "YamlHelper.h"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include <stdexcept>

/**
 * Registration of other Oatpp components.
 * Declarative service executors are registered here.
 */
class OtherComponent : public AbstractComponentReg
{
	OATPP_CREATE_COMPONENT(
		std::shared_ptr<oatpp::web::client::HttpRequestExecutor>,
		diagnosisApiExecutor)("diagnosis-api", [] {
			YAML::Node config = YAML::LoadFile("./conf/third-services.yaml");
			YamlHelper yaml;
			const std::string host = yaml.getString(
				&config,
				"declarative-services.diagnosis-api.host");
			const std::string portText = yaml.getString(
				&config,
				"declarative-services.diagnosis-api.port");
			const int port = portText.empty() ? 0 : std::stoi(portText);
			if (host.empty() || port <= 0 || port > 65535)
			{
				throw std::runtime_error(
					"Invalid declarative-services.diagnosis-api configuration.");
			}

			auto connectionProvider =
				oatpp::network::tcp::client::ConnectionProvider::createShared(
					{
						host.c_str(),
						static_cast<v_uint16>(port),
						oatpp::network::Address::IP_4
					});
			return oatpp::web::client::HttpRequestExecutor::createShared(
				connectionProvider);
		}());
};

#endif // _OTHERCOMMPONENT_H_
