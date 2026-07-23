package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.entity.OrderTemplate;
import com.zeroone.star.configuration.query.OrderTemplateNameQuery;
import com.zeroone.star.configuration.query.OrderTemplateQuery;
import com.zeroone.star.configuration.vo.OrderTemplateDetailVO;
import com.zeroone.star.configuration.vo.OrderTemplateListVO;
import com.zeroone.star.configuration.vo.OrderTemplateNameVO;

import java.util.List;

public interface IOrderTemplateService extends IService<OrderTemplate> {

    List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query);

    Page<OrderTemplateListVO> list(OrderTemplateQuery query);

    OrderTemplateDetailVO getDetail(String id);
}
