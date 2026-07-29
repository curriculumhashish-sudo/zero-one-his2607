#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2023/02/22 20:18:53

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
#include "jwt-cpp/jwt.h"
#include <list>

/**
 * ������Ϣ��ȡ״̬����
 */
enum class PayloadCode
{
	// ��Ϣ��֤�����ɹ�
	SUCCESS,
	// Token�ѹ���
	TOKEN_EXPIRED_ERROR,
	// ǩ����ʽ����
	SIGNATUREFORMAT_ERROR,
	// ���ܴ���
	DECODE_ERROR,
	// ��֤����
	VERIFICATION_ERROR,
	// ��������
	OTHER_ERROR
};

/**
 * ������Ϣʵ����
 */
class PayloadDTO
{
public:
	// ��ȡ״̬���Ӧ��ö��ֵ����
	static std::string getCodeName(PayloadCode code);
	// ��ȡƾ֤ǰ׺
	static std::string getTokenPrefix();
private:
	// Token�ַ�����Ӧ��ֵ
	std::string token;
	// ���ڣ�һ���ͻ���id����
	std::string aud;
	// ǩ����
	std::string iss;
	// ����,һ����û���
	std::string sub;
	// ����ʱ�䣨ע�⣺����ƾ֤��ʱ��������ָ����Ч����������һ��Unixʱ���
	int64_t exp;
	// ǩ��ʱ�䣬��һ��Unixʱ���
	int64_t iat;
	// ��Чʱ�䣬��һ��Unixʱ���
	int64_t nbf;
	// jwt id Ψһ��ʶ
	std::string jti;
	// ����״̬ϵ��Ϣ
	PayloadCode code;
	// �û����
	std::string id;
	// �û�ӵ�е�Ȩ��
	std::list<std::string> authorities;
	// TIP�����������ֶ��ں������Ӽ���
public:
	PayloadDTO();
	/**
	 * �����ʼ��
	 * @param username �û���
	 * @param exp ����ʱ�䣬��λΪ��
	 * @param _authorities Ȩ���б�
	 */
	PayloadDTO(std::string username, int64_t exp, std::list<std::string> _authorities);
	// ��ȡToken�ַ���
	std::string getToken() const { return token; }
	// ����Token�ַ���
	void setToken(std::string val) { token = val; }
	// ��ȡ����״̬����Ϣ
	PayloadCode getCode() const { return code; }
	// ��������״̬����Ϣ
	void setCode(PayloadCode val) { code = val; }
	
	// ��׼�����ֶε�getter/setter
	std::string Aud() const { return aud; }
	void Aud(std::string val) { aud = val; }
	std::string Iss() const { return iss; }
	void Iss(std::string val) { iss = val; }
	std::string Sub() const { return sub; }
	void Sub(std::string val) { sub = val; }
	int64_t Exp() const { return exp; }
	void Exp(int64_t val) { exp = val; }
	int64_t Iat() const { return iat; }
	void Iat(int64_t val) { iat = val; }
	int64_t Nbf() const { return nbf; }
	void Nbf(int64_t val) { nbf = val; }
	std::string Jti() const { return jti; }
	void Jti(std::string val) { jti = val; }

	// �Զ��������ֶε�getter/setter
	std::string getUsername() const { return Sub(); }
	void setUsername(std::string val) { Sub(val); }
	std::string getId() const { return id; }
	void setId(std::string val) { id = val; }
	const std::list<std::string>& getAuthorities() const { return authorities; }
	void setAuthorities(std::list<std::string> val) { authorities = val; }
	// ע�⣺���������ֶκ���Ҫ����getter/setter

	// ����Ȩ��
	void putAuthority(std::string authstr);
	
	// ��Payload������ת����jwt builder��
	// ע�⣺���������ֶκ���Ҫά���˷����Ķ���
	void propToJwt(jwt::builder<jwt::default_clock, jwt::traits::kazuho_picojson>& builder) const;

	// ��jwt decoded_jwt������ת����Payload��
	// ע�⣺���������ֶκ���Ҫά���˷����Ķ���
	void propToPayload(const jwt::decoded_jwt<jwt::traits::kazuho_picojson>& decoded);
};

