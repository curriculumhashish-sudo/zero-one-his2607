package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.DeptStockDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：取药科室库存Mapper（映射wkf_inventory_item表）
 * </p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface DeptStockMapper extends BaseMapper<DeptStockDO> {

    /**
     * 根据ID查询库存（指定字段）
     */
    @Select("SELECT id, category_code, item_table, item_id, name, py_str, wb_str, " +
            "inventory_status_enum, supplier_id, description_text, unit_code, quantity, " +
            "characteristic_json, lot_number, production_date, expiration_date, validity_mon, " +
            "location_store_id, location_id, trace_no, packaging_levels " +
            "FROM wkf_inventory_item WHERE id = #{id} AND is_deleted = 0")
    DeptStockDO selectByIdWithFields(@Param("id") String id);

    /**
     * 查询库存低于安全库存的记录
     */
    @Select("SELECT id, name, item_id, quantity, unit_code, location_id " +
            "FROM wkf_inventory_item WHERE is_deleted = 0 AND quantity < 10")
    List<DeptStockDO> selectLowStockList();
}
