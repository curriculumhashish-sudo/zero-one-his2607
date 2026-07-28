package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.DispensingItem;

/**
 * <p>
 * 描述：发药明细表 Mapper 接口，对应数据库表 {@code dispensing_item}
 * </p>
 * <p>
 * 功能说明：提供发药明细表的数据库操作能力，包括插入、更新、删除、查询等。
 * 继承 MyBatis-Plus 的 BaseMapper，无需手动编写 SQL 即可完成单表 CRUD。
 * 在查看发药药品项目时，通过该 Mapper 查询指定发药单据下的药品明细。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface DispensingItemMapper extends BaseMapper<DispensingItem> {
}
