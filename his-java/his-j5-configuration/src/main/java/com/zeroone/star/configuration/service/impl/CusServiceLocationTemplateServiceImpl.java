package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.CusServiceLocationTemplate;
import com.zeroone.star.configuration.mapper.CusServiceLocationTemplateMapper;
import com.zeroone.star.configuration.service.ICusServiceLocationTemplateService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Map;

@Service
public class CusServiceLocationTemplateServiceImpl extends ServiceImpl<CusServiceLocationTemplateMapper, CusServiceLocationTemplate> implements ICusServiceLocationTemplateService {

    @Autowired
    private CusServiceLocationTemplateMapper cusServiceLocationTemplateMapper;
    @Override
    public IPage<CusServiceLocationTemplate> listPage(Page<CusServiceLocationTemplate> page, Map<String, Object> params) {
        LambdaQueryWrapper<CusServiceLocationTemplate> wrapper = new LambdaQueryWrapper<>();
        
        if (params.get("organizationId") != null && !params.get("organizationId").toString().isEmpty()) {
            wrapper.eq(CusServiceLocationTemplate::getOrganizationId, params.get("organizationId"));
        }
        if (params.get("itemTable") != null && !params.get("itemTable").toString().isEmpty()) {
            wrapper.eq(CusServiceLocationTemplate::getItemTable, params.get("itemTable"));
        }
        if (params.get("distributionCategoryCode") != null && !params.get("distributionCategoryCode").toString().isEmpty()) {
            wrapper.eq(CusServiceLocationTemplate::getDistributionCategoryCode, params.get("distributionCategoryCode"));
        }
        if (params.get("defLocationId") != null && !params.get("defLocationId").toString().isEmpty()) {
            wrapper.eq(CusServiceLocationTemplate::getDefLocationId, params.get("defLocationId"));
        }
        if (params.get("itemDefinitionId") != null && !params.get("itemDefinitionId").toString().isEmpty()) {
            wrapper.eq(CusServiceLocationTemplate::getItemDefinitionId, params.get("itemDefinitionId"));
        }
        
        wrapper.orderByAsc(CusServiceLocationTemplate::getPriority);
        wrapper.orderByAsc(CusServiceLocationTemplate::getDisplayOrder);
        
        return page(page, wrapper);
    }

    @Override
    public boolean saveTemplate(CusServiceLocationTemplate template) {
        LambdaQueryWrapper<CusServiceLocationTemplate> wrapper = Wrappers.lambdaQuery();
        wrapper.eq(CusServiceLocationTemplate::getOrganizationId, template.getOrganizationId())
               .eq(CusServiceLocationTemplate::getItemTable, template.getItemTable())
               .eq(CusServiceLocationTemplate::getDistributionCategoryCode, template.getDistributionCategoryCode())
               .eq(CusServiceLocationTemplate::getDefLocationId, template.getDefLocationId());
        
        CusServiceLocationTemplate existing = cusServiceLocationTemplateMapper.selectOne(wrapper);
        
        if (existing != null) {
            template.setId(existing.getId());
            template.setVersion(existing.getVersion());
            return this.updateById(template);
        } else {
            return this.save(template);
        }
    }
}