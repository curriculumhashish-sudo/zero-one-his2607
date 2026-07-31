package com.zeroone.star.ons.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.ons.entity.PatientConsumableItemDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：患者耗材项目Mapper（避免select *）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface PatientConsumableItemMapper extends BaseMapper<PatientConsumableItemDO> {

    /**
     * 根据患者ID查询耗材项目列表（指定字段，避免select *）
     */
    @Select("SELECT id, bus_no, item_id, item_name, item_code, consumable_type, " +
            "quantity, unit_code, unit_name, unit_price, total_price, " +
            "stock_quantity, stock_unit, manufacturer, batch_no, expiry_date, " +
            "exec_dept_id, exec_dept_name, status, execution_status, remark " +
            "FROM patient_consumable_item WHERE patient_id = #{patientId} AND is_deleted = 0 " +
            "ORDER BY created_at DESC")
    List<PatientConsumableItemDO> selectByPatientId(@Param("patientId") String patientId);
}
