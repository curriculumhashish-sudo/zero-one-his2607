package com.zeroone.star.clinics.service;

import com.zeroone.star.clinics.entity.WkfSlot;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.j8.clinics.RetireScheduleDTO;
import com.zeroone.star.project.dto.j8.clinics.SlotBatchReleaseDTO;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.j8.clinics.ResumeScheduleVO;
import com.zeroone.star.project.vo.j8.clinics.SlotBatchReleaseVO;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import jakarta.validation.Valid;

import java.util.List;

/**
 * <p>
 * 号源时段（FHIR 时段 语义） 服务类
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
public interface IWkfSlotService extends IService<WkfSlot> {

    /**
     * 获取已放号源列表
     *
     * @param query 查询参数
     * @return 放号源列表
     */
    List<SlotVO> getRelNumberSource(@Valid RelSlotQuery query);


    /**
     * 删除号源
     *
     * @param slotId 待删除的号源时段主键id
     */
    void deleteNumberSource(String slotId);

    /**
     * 批量放号
     *
     * @param numberSourceBatchReleaseDTO 批量待放号参数
     * @return 响应对象
     */
    SlotBatchReleaseVO batchReleaseNumberSource(@Valid SlotBatchReleaseDTO numberSourceBatchReleaseDTO);

    /**
     * 恢复开诊
     *
     * @param scheduleId 排班id
     * @return 响应对象
     */
    ResumeScheduleVO reactivateSchedule(String scheduleId);

    /**
     * 作废日程
     *
     * @param retireScheduleDTO 作废日程参数
     */
    void retireSchedule(@Valid RetireScheduleDTO retireScheduleDTO);
}
