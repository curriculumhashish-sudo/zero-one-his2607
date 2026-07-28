package com.zeroone.star.clinics.service.impl;

import com.zeroone.star.clinics.entity.WkfAppointmentQueue;
import com.zeroone.star.clinics.mapper.WkfAppointmentQueueMapper;
import com.zeroone.star.clinics.service.IWkfAppointmentQueueService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 预约与候诊队列（FHIR 预约 语义） 服务实现类
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Service
public class WkfAppointmentQueueServiceImpl extends ServiceImpl<WkfAppointmentQueueMapper, WkfAppointmentQueue> implements IWkfAppointmentQueueService {

}
