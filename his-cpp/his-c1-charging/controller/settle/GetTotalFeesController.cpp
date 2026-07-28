#include "GetTotalFeesController.h"
// 请确保这个路径能正确找到上面的 GetTotalFeesService.h
#include "../../service/settle/GetTotalFeesService.h" 

GetTotalFeesJsonVO::Wrapper GetTotalFeesController::execQueryGetTotalFees(const GetTotalFeesQuery::Wrapper& query)
{
    // 1. 创建 VO 对象
    auto vo = GetTotalFeesJsonVO::createShared();

    // 2. 实例化 Service
    GetTotalFeesService service;

    // 3. 调用 Service 获取 DTO 列表
    auto dtolist = service.listAll(query);

    // 4. 使用 VO 的 success 方法包装数据
    vo->success(dtolist);

    return vo;
}

