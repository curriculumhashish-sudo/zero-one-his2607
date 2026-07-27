package com.zeroone.star.clinics.service;

import com.zeroone.star.clinics.entity.WkfSlot;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
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
}
