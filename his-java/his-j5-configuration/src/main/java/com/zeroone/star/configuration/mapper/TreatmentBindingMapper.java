package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.TreatmentBinding;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 * 描述：诊疗绑定Mapper（映射cus_service_device_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Mapper
public interface TreatmentBindingMapper extends BaseMapper<TreatmentBinding> {

    /**
     * 根据项目ID统计绑定数量
     */
    @Select("SELECT COUNT(1) FROM cus_service_device_template WHERE item_id = #{itemId} AND is_deleted = 0")
    Long countByItemId(@Param("itemId") String itemId);
}
