package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.UsageBinding;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 * 描述：用法绑定Mapper（映射cus_service_location_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Mapper
public interface UsageBindingMapper extends BaseMapper<UsageBinding> {

    /**
     * 根据项目定义ID统计绑定数量
     */
    @Select("SELECT COUNT(1) FROM cus_service_location_template WHERE item_definition_id = #{itemDefinitionId} AND is_deleted = 0")
    Long countByItemDefinitionId(@Param("itemDefinitionId") String itemDefinitionId);
}
