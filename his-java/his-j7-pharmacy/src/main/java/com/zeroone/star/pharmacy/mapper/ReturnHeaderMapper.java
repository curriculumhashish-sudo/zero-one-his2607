package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.ReturnHeader;

/**
 * <p>
 * 描述：退药申请主表 Mapper 接口，对应数据库表 {@code return_header}
 * </p>
 * <p>
 * 功能说明：提供退药申请主表的数据库操作能力。
 * 在住院退药确认和门诊拒绝退药流程中，通过该 Mapper 更新退药单状态；
 * 在分页查询退药患者队列时，通过该 Mapper 进行条件查询。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface ReturnHeaderMapper extends BaseMapper<ReturnHeader> {
}
