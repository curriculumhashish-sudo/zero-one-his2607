package com.zeroone.star.j3.storehouse.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.storehouse.entity.StoreHouseDO;
import org.apache.ibatis.annotations.Mapper;
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
}
