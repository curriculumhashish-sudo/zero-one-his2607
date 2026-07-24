package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.SummaryOrder;

/**
 * <p>
 * 描述：汇总单表 Mapper 接口，对应数据库表 {@code summary_order}
 * </p>
 * <p>
 * 功能说明：提供汇总单表的数据库操作能力。
 * 在住院发药流程中，通过该 Mapper 查询待发药的汇总单队列；
 * 在发药驳回时，通过该 Mapper 同步更新汇总单状态为"已驳回"。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface SummaryOrderMapper extends BaseMapper<SummaryOrder> {
}
