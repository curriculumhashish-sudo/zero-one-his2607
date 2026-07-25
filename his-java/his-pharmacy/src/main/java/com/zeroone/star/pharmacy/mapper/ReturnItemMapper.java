package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.ReturnItem;

/**
 * <p>
 * 描述：退药申请明细表 Mapper 接口，对应数据库表 {@code return_item}
 * </p>
 * <p>
 * 功能说明：提供退药申请明细表的数据库操作能力。
 * 在查看退药明细时，通过该 Mapper 查询指定退药单下的药品明细；
 * 在确认退药回补库存时，通过该 Mapper 获取勾选药品的详细信息。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface ReturnItemMapper extends BaseMapper<ReturnItem> {
}
