package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Mapper;

/**
 * G7 入库单审批流转 Mapper。
 *
 * @author lvv
 * @version 1.0.0
 */
@Mapper
public interface InboundApprovalMapper extends BaseMapper<WkfSupplyRequest> {
}
