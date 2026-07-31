package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.DeptStockDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.math.BigDecimal;
import java.util.List;

/**
 * <p>
 * 描述：取药科室库存Mapper（避免select *，指定字段）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface DeptStockMapper extends BaseMapper<DeptStockDO> {

    /**
     * 根据科室ID和物品ID查询库存（指定字段）
     */
    @Select("SELECT id, dept_id, dept_name, item_id, item_name, item_code, item_type, " +
            "specification, unit, stock_quantity, safety_stock, is_enabled, remark " +
            "FROM dept_stock WHERE id = #{id} AND is_deleted = 0")
    DeptStockDO selectByIdWithFields(@Param("id") String id);

    /**
     * 查询库存低于安全库存的记录
     */
    @Select("SELECT id, dept_id, dept_name, item_id, item_name, item_code, item_type, " +
            "stock_quantity, safety_stock " +
            "FROM dept_stock WHERE is_deleted = 0 AND stock_quantity < safety_stock")
    List<DeptStockDO> selectLowStockList();
}
