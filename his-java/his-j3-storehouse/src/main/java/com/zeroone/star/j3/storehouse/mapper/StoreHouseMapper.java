package com.zeroone.star.j3.storehouse.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.storehouse.entity.StoreHouseDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * 库房药房 Mapper
 * @author mie
 * @version 1.0.0
 */
@Mapper
public interface StoreHouseMapper extends BaseMapper<StoreHouseDO> {

    /**
     * 查询最大业务编号（加行锁，防止并发重复）
     */
    @Select("SELECT bus_no FROM adm_location WHERE is_deleted = false AND form_enum IN ('WAREHOUSE','PHARMACY','DEPT_PHARMACY') AND bus_no LIKE 'LO%' ORDER BY bus_no DESC LIMIT 1 FOR UPDATE")
    String selectMaxBusNoForUpdate();

    /**
     * 统计指定库房药房下仍有库存（数量>0）的库存项数量
     */
    @Select("SELECT COUNT(*) FROM wkf_inventory_item WHERE is_deleted = 0 AND location_id = #{locationId} AND quantity > 0")
    long countInventoryByLocationId(@Param("locationId") String locationId);

    /**
     * 统计引用指定库房药房的未完成入出库/供应单据数量
     */
    @Select("SELECT COUNT(*) FROM wkf_supply_request WHERE is_deleted = 0 " +
            "AND (source_location_id = #{locationId} OR purpose_location_id = #{locationId}) " +
            "AND status_enum NOT IN ('COMPLETED','CANCELLED','REJECTED')")
    long countUnfinishedSupplyRequestByLocationId(@Param("locationId") String locationId);

    /**
     * 统计指定库房药房下未完成的发药记录数量
     */
    @Select("SELECT COUNT(*) FROM med_medication_dispense WHERE is_deleted = 0 " +
            "AND location_id = #{locationId} " +
            "AND status_enum NOT IN ('COMPLETED','CANCELLED','ENTERED_IN_ERROR','STOPPED')")
    long countUnfinishedDispenseByLocationId(@Param("locationId") String locationId);
}
