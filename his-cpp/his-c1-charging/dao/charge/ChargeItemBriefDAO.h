#pragma once

#ifndef _CHARGE_ITEM_BRIEF_DAO_H_
#define _CHARGE_ITEM_BRIEF_DAO_H_

#include "BaseDAO.h"
#include "domain/do/charge/ChargeItemBriefDO.h"

/**
 * 收费项摘要查询 DAO
 * 按就诊ID列表批量查询收费项摘要
 */
class ChargeItemBriefDAO : public BaseDAO
{
public:
    /**
     * 按就诊ID列表批量查询收费项摘要
     * 用于分页后加载 cashierChargeItems[] 子列表
     * @param encounterIds 主查询分页后的就诊ID列表
     * @return 收费项摘要 DO 列表（调用方按 encounterId 分组）
     */
    std::list<ChargeItemBriefDO> selectByEncounterIds(const std::list<std::string>& encounterIds);
};

#endif // !_CHARGE_ITEM_BRIEF_DAO_H_
