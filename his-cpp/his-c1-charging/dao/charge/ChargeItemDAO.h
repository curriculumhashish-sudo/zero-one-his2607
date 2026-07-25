#pragma once

#ifndef _CHARGE_ITEM_DAO_H_
#define _CHARGE_ITEM_DAO_H_

#include "BaseDAO.h"
#include "domain/do/charge/ChargeItemDO.h"

/**
 * 收费项明细查询 DAO（接口2）
 */
class ChargeItemDAO : public BaseDAO
{
public:
    /**
     * 按就诊ID查询收费项明细列表
     * @param encounterId 就诊主键
     * @return 收费项明细 DO 列表
     */
    std::list<ChargeItemDO> selectByEncounterId(const std::string& encounterId);
};

#endif // !_CHARGE_ITEM_DAO_H_
