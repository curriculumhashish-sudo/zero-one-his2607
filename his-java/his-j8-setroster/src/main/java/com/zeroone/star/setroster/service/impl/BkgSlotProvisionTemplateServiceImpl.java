package com.zeroone.star.setroster.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.setroster.entity.BkgSlotProvisionTemplate;
import com.zeroone.star.setroster.mapper.BkgSlotProvisionTemplateMapper;
import com.zeroone.star.setroster.service.IBkgSlotProvisionTemplateService;
import org.springframework.stereotype.Service;

/**
 * T1 放号时段模板 服务实现类
 *
 * @author Gin
 * @since 2026-07-24
 */
@Service
public class BkgSlotProvisionTemplateServiceImpl
        extends ServiceImpl<BkgSlotProvisionTemplateMapper, BkgSlotProvisionTemplate>
        implements IBkgSlotProvisionTemplateService {
}
