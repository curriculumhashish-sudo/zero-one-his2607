package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.DispensingHeader;

/**
 * <p>
 * 描述：发药单据主表 Mapper 接口，对应数据库表 {@code dispensing_header}
 * </p>
 * <p>
 * 功能说明：提供发药单据主表的数据库操作能力，包括插入、更新、删除、查询等。
 * 继承 MyBatis-Plus 的 BaseMapper，无需手动编写 SQL 即可完成单表 CRUD。
 * 在住院发药业务流程中，通过该 Mapper 更新发药单据状态（待发药→已发药/已驳回）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface DispensingHeaderMapper extends BaseMapper<DispensingHeader> {
}
