package com.zeroone.star.clinics.service.impl;

import com.zeroone.star.clinics.entity.BkgStopClinicSlotFreeze;
import com.zeroone.star.clinics.mapper.BkgStopClinicSlotFreezeMapper;
import com.zeroone.star.clinics.service.IBkgStopClinicSlotFreezeService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 停诊预览期间源号源冻结（防并发新约） 服务实现类
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Service
public class BkgStopClinicSlotFreezeServiceImpl extends ServiceImpl<BkgStopClinicSlotFreezeMapper, BkgStopClinicSlotFreeze> implements IBkgStopClinicSlotFreezeService {

}
