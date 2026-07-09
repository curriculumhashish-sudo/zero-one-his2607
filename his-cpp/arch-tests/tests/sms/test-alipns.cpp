/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/05/14 14:52:30

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
#ifdef USE_ALISMS

#include "sms/aliyun/AliPnsSender.h"

#include "gtest/gtest.h"
#include <iostream>

// 测试发送验证码
TEST(AliPnsTest, SendVerifyCode)
{
	AliPnsSender sender;
	// 注意：参数中如有中文字符，需确保从中文词典获取，否则可能发送失败
	std::string phoneNumber = "";
	std::string templateCode = "100005";
	std::string signName = "云渚科技验证平台";
	std::string templateParam = "{\"code\":\"##code##\", \"min\":\"5\"}";
	std::shared_ptr<AliPnsResult> res = sender.sendVerifyCode(phoneNumber, signName, templateCode, templateParam);
	ASSERT_EQ(res->Code(), "OK");
	std::cout
		<< "code:  " << res->Code() << std::endl
		<< "msg:   " << res->Message() << std::endl
		<< "bizid: " << res->BizId() << std::endl
		<< "reqid: " << res->RequestId() << std::endl;
}

// 测试校验验证码
TEST(AliPnsTest, CheckVerifyCode)
{
	return;
	AliPnsSender sender;
	std::string phoneNumber = "";
	std::string verifyCode = "6081";
	std::shared_ptr<AliPnsResult> res = sender.checkVerifyCode(phoneNumber, verifyCode);
	ASSERT_EQ(res->Code(), "OK");
	std::cout
		<< "code:   " << res->Code() << std::endl
		<< "msg:    " << res->Message() << std::endl
		<< "bizid:  " << res->BizId() << std::endl
		<< "reqid:  " << res->RequestId() << std::endl
		<< "verify: " << res->VerifyResult() << std::endl;
}

#endif