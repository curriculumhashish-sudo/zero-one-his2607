package com.zeroone.star.calendar.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.calendar.entity.WkfSlot;
import com.zeroone.star.project.dto.j8.calendar.PractitionerOptionDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDetailDTO;
import com.zeroone.star.project.query.j8.calendar.SlotQuery;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 号源时段 Mapper 接口
 * </p>
 * @author j8组
 * @since 2026-07-25
 */
public interface WkfSlotMapper extends BaseMapper<WkfSlot> {

    /**
     * 分页查询号源列表（多条件 + 联表查名称）
     *
     * @param page  分页对象
     * @param query 查询条件（科室、日期、医师、状态）
     * @return 分页号源列表
     */
    Page<SlotDTO> selectSlotPage(Page<SlotDTO> page, @Param("query") SlotQuery query);

    /**
     * 查询单个号源详情（比列表多备注字段）
     *
     * @param slotId 号源主键
     * @return 号源详情，不存在返回 null
     */
    SlotDetailDTO selectSlotDetail(@Param("slotId") String slotId);

    /**
     * 查询该科室下有排班的医师列表
     *
     * @param organizationId 科室主键
     * @return 医师下拉选项
     */
    List<PractitionerOptionDTO> selectPractitioners(@Param("organizationId") String organizationId);
}
