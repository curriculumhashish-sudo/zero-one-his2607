#include "stdafx.h"
#include "exportInfoController.h"

using oatpp::web::server::api::ApiController;

// 明确指定 OutgoingResponse 和 String 的命名空间，确保与头文件一致
std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse> ExportInfoController::execDownloadFile(const String& filename)
{
    return nullptr;
}
