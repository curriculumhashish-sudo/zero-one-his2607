// ============================================================================
// ExecuteRefundDAO.h — 执行退费 — 数据访问对象
// ============================================================================
// 实际写操作通过 Service 层直接调用 BaseDAO insert/update
// 本 DAO 保留以备后续需要 DB 级写操作时扩展

#pragma once
#include "BaseDAO.h"
#include "../../domain/do/refund/ExecuteRefundDO.h"

class ExecuteRefundDAO : public BaseDAO
{
public:
    // 插入退费冲正记录（预留，实际逻辑在 Service 层）
    uint64_t insertRefundRecord(const ExecuteRefundDO& data);
};
