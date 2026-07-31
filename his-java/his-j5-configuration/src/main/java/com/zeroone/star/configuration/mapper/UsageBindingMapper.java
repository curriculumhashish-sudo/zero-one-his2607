package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.UsageBinding;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 * 描述：用法绑定Mapper（避免select *）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Mapper
public interface UsageBindingMapper extends BaseMapper<UsageBinding> {

    /**
     * 根据用法ID统计绑定数量
     */
    @Select("SELECT COUNT(1) FROM t_usage_binding WHERE usage_id = #{usageId} AND is_deleted = 0")
    Long countByUsageId(@Param("usageId") Long usageId);
}
