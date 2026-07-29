package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import org.apache.ibatis.annotations.Mapper;

/**
 * 药房入库单 Mapper 接口
 */
@Mapper
public interface InStockOrderMapper extends BaseMapper<InStockOrder> {
    // 基础分页查询 MyBatis-Plus 已自带
    // 若有复杂的连表查询（如关联用户表查姓名），可在此处自定义方法并在 XML 中实现
}