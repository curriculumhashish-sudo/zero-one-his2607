/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/05/15 11:27:44

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
#include "pch.h"
#include "../../../include/domain/dto/PayloadDTO.h"

std::string PayloadDTO::getCodeName(PayloadCode code)
{
	switch (code)
	{
	case PayloadCode::SUCCESS:
		return "SUCCESS";
	case PayloadCode::TOKEN_EXPIRED_ERROR:
		return "TOKEN_EXPIRED_ERROR";
	case PayloadCode::SIGNATUREFORMAT_ERROR:
		return "SIGNATUREFORMAT_ERROR";
	case PayloadCode::DECODE_ERROR:
		return "DECODE_ERROR";
	case PayloadCode::VERIFICATION_ERROR:
		return "VERIFICATION_ERROR";
	case PayloadCode::OTHER_ERROR:
		return "OTHER_ERROR";
	default:
		return "NONE";
	}
}

std::string PayloadDTO::getTokenPrefix()
{
	return "Bearer ";
}

PayloadDTO::PayloadDTO(std::string username, int64_t _exp, std::list<std::string> _authorities) :PayloadDTO()
{
	this->sub = username;
	this->exp = _exp;
	this->authorities = _authorities;
}

PayloadDTO::PayloadDTO()
{
	this->exp = 36000;
	this->Aud("manager");
	this->Iss("01xq");
	this->Sub("none");
	// 设置jti 使用当前时间戳作为jti的值
	this->Jti(std::to_string(time(nullptr)));
	this->setCode(PayloadCode::SUCCESS);
	// 新增：租户标识默认值
	this->tenantId = "TEST_TENANT_001";
	// 新增：机构标识默认值
	this->orgId = "TEST_ORG_001";
}

void PayloadDTO::putAuthority(std::string authstr)
{
	this->authorities.emplace_back(authstr);
}

void PayloadDTO::propToJwt(jwt::builder<jwt::default_clock, jwt::traits::kazuho_picojson>& builder) const
{
	// 转换权限列表
	picojson::array auth_array;
	// 将std::list转换为picojson::array
	for (const auto& auth : authorities) {
		auth_array.push_back(picojson::value(auth));
	}
	builder.set_payload_claim("roles", jwt::basic_claim<jwt::traits::kazuho_picojson>(auth_array));
	// 转换id
	builder.set_payload_claim("id", jwt::basic_claim<jwt::traits::kazuho_picojson>(id));
	// 新增：转换租户标识
	builder.set_payload_claim("tenantId", jwt::basic_claim<jwt::traits::kazuho_picojson>(tenantId));
	// 新增：转换机构标识
	builder.set_payload_claim("orgId", jwt::basic_claim<jwt::traits::kazuho_picojson>(orgId));
}

void PayloadDTO::propToPayload(const jwt::decoded_jwt<jwt::traits::kazuho_picojson>& decoded)
{
	// 获取payload claims (返回picojson::object)
	const auto& claims = decoded.get_payload_json();
	// 转换权限列表
	if (claims.count("roles")) {
		const auto& auth_value = claims.at("roles");
		// 从picojson数组中提取
		if (auth_value.is<picojson::array>()) {
			try {
				auto arr = auth_value.get<picojson::array>();
				for (const auto& item : arr) {
					if (item.is<std::string>()) {
						putAuthority(item.get<std::string>());
					}
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Error parsing authorities: " << e.what() << std::endl;
			}
		}
	}
	// 转换id（数字类型或字符串类型）
	if (claims.count("id")) {
		const auto& id_value = claims.at("id");
		try {
			// 尝试作为字符串
			if (id_value.is<std::string>()) {
				setId(id_value.get<std::string>());
			}
			// 尝试作为整数
			else if (id_value.is<int64_t>()) {
				setId(std::to_string(id_value.get<int64_t>()));
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error parsing id: " << e.what() << std::endl;
		}
	}
	// 新增：解析租户标识
	if (claims.count("tenantId")) {
		const auto& value = claims.at("tenantId");
		try {
			if (value.is<std::string>()) {
				this->tenantId = value.get<std::string>();
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error parsing tenantId: " << e.what() << std::endl;
			this->tenantId = "TEST_TENANT_001";
		}
	}
	// 新增：解析机构标识
	if (claims.count("orgId")) {
		const auto& value = claims.at("orgId");
		try {
			if (value.is<std::string>()) {
				this->orgId = value.get<std::string>();
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error parsing orgId: " << e.what() << std::endl;
			this->orgId = "TEST_ORG_001";
		}
	}
}