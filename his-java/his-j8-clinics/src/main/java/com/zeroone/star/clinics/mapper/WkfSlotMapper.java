package com.zeroone.star.clinics.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.clinics.entity.WkfSlot;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 号源时段（FHIR 时段 语义） Mapper 接口
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
public interface WkfSlotMapper extends BaseMapper<WkfSlot> {

    /**
     * 获取已放号源列表
     * @param query 查询参数
     * @return 号源VO列表
     */
    List<SlotVO> selectRelSlots(@Param("query") RelSlotQuery query);
}
