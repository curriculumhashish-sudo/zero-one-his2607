package com.zeroone.star.calendar.service;

import com.zeroone.star.project.dto.j8.calendar.SlotBlockDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDetailDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotOverbookDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotPageResultDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotUnblockDTO;
import com.zeroone.star.project.query.j8.calendar.SlotQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 号源日历 服务接口
 * </p>
 * @author j8组
 * @since 2026-07-26
 */
public interface ISlotCalendarService {

    /**
     * 获取号源列表（分页 + 多条件查询 + 聚合统计）
     * @param query 查询条件
     * @return 分页号源列表
     */
    JsonVO<SlotPageResultDTO> pageSlots(SlotQuery query);

    /**
     * 获取号源详情
     * @param slotId 号源主键
     * @return 号源详情
     */
    JsonVO<SlotDetailDTO> getSlotDetail(String slotId);

    /**
     * 批量封锁号源
     * @param dto 封锁参数
     * @return 操作结果
     */
    JsonVO<String> blockSlots(SlotBlockDTO dto);

    /**
     * 批量解封号源
     * @param dto 解封参数
     * @return 操作结果
     */
    JsonVO<String> unblockSlots(SlotUnblockDTO dto);

    /**
     * 标记超售
     * @param dto 超售参数
     * @return 操作结果
     */
    JsonVO<String> setOverbooked(SlotOverbookDTO dto);
}
