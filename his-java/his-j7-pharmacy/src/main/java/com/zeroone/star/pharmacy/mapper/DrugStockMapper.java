package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.DrugStock;

/**
 * <p>
 * 描述：药品库存表 Mapper 接口，对应数据库表 {@code drug_stock}
 * </p>
 * <p>
 * 功能说明：提供药品库存表的数据库操作能力。
 * 在住院退药确认流程中，通过该 Mapper 查询药品当前库存，
 * 然后更新库存数量（原库存 + 退药数量），实现库存回补。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface DrugStockMapper extends BaseMapper<DrugStock> {
}
