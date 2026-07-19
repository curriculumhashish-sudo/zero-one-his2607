#pragma once

#ifndef EXPORTINFO_CONTROLLER_H
#define EXPORTINFO_CONTROLLER_H

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/cost_info/exportDTO.h"
#include "domain/vo/cost_info/exportVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin ApiController codegen section

#define API_TAG  "test aaa" 
// ZH_WORDS_GETTER("file.tag")

class ExportInfoController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(ExportInfoController);
public:
	// 定义一个文件下载接口
	// 定义描述
	API_DEF_ENDPOINT_INFO(
		ZH_WORDS_GETTER("file.download.summary"), downloadFile, Void, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "filename", ZH_WORDS_GETTER("file.field.filename"), "file/test.jpg", true);
	);
	// 定义端点
	API_HANDLER_ENDPOINT_OPTION(API_M_GET, "/file/download", downloadFile, QUERY(String, filename), return execDownloadFile(filename););
private: 

	// 执行文件下载处理
	std::shared_ptr<OutgoingResponse> execDownloadFile(const String& filename);
};
#include OATPP_CODEGEN_END(ApiController) ///< End ApiController codegen section
#endif