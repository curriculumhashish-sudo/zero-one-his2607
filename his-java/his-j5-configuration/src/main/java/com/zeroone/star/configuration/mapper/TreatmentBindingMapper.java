package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.TreatmentBinding;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 * 描述：诊疗绑定Mapper（避免select *）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Mapper
public interface TreatmentBindingMapper extends BaseMapper<TreatmentBinding> {

    /**
     * 根据诊疗项目ID统计绑定数量
     */
    @Select("SELECT COUNT(1) FROM t_treatment_binding WHERE treatment_id = #{treatmentId} AND is_deleted = 0")
    Long countByTreatmentId(@Param("treatmentId") Long treatmentId);
}
