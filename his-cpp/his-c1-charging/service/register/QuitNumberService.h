#pragma once
#ifndef _QUIT_NUMBER_SERVICE_
#define _QUIT_NUMBER_SERVICE_

#include "../../dao/register/QuitNumberDAO.h"
#include "../../domain/dto/register/QuitNumberDTO.h" 
#include <string>

/**
 * 确认退号服务
 * 职责：协调DAO操作，完成退号全流程（更新状态、释放号源、退款记录等）
 */
class QuitNumberService {
private:
    QuitNumberDAO m_dao; // 数据访问对象实例

public:
    /**
     * 确认退号
     * @param request 退号请求DTO（包含就诊记录ID、版本号、退款信息等）
     * @return 操作结果（0=成功，负数=失败）
     */
    bool confirmQuitNumber(const oatpp::Object<QuitNumberDTO>& request, const PayloadDTO& payload);
};

#endif // !_QUIT_NUMBER_SERVICE_
