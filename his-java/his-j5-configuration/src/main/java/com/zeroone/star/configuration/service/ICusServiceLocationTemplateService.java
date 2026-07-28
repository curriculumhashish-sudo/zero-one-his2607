package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.entity.CusServiceLocationTemplate;

import java.util.Map;

public interface ICusServiceLocationTemplateService extends IService<CusServiceLocationTemplate> {

    IPage<CusServiceLocationTemplate> listPage(Page<CusServiceLocationTemplate> page, Map<String, Object> params);

    boolean saveTemplate(CusServiceLocationTemplate template);
}