package com.zeroone.star.clinics.service.impl;

import com.zeroone.star.clinics.entity.WkfSlot;
import com.zeroone.star.clinics.mapper.WkfSlotMapper;
import com.zeroone.star.clinics.service.IWkfSlotService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 * 号源时段（FHIR 时段 语义） 服务实现类
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Service
public class WkfSlotServiceImpl extends ServiceImpl<WkfSlotMapper, WkfSlot> implements IWkfSlotService {

    /**
     * 获取已放号源列表
     * 多表联查 wkf_slot + wkf_schedule + wkf_schedule_actor + adm_practitioner + adm_healthcare_service + adm_organization
     *
     * @param query 查询参数
     * @return 放号源列表
     */
    @Override
    public List<SlotVO> getRelNumberSource(RelSlotQuery query) {
        return baseMapper.selectRelSlots(query);
    }
}
