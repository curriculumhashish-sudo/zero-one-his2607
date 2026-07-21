package com.zeroone.star.hisj5basedata.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.hisj5basedata.entity.OrderTemplate;
import com.zeroone.star.hisj5basedata.query.OrderTemplateNameQuery;
import com.zeroone.star.hisj5basedata.query.OrderTemplateQuery;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateDetailVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateListVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateNameVO;

import java.util.List;

/**
 * <p>
 * 组套模板 服务类
 * </p>
 * @author keepasdd
 */
public interface IOrderTemplateService extends IService<OrderTemplate> {


    List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query);


    Page<OrderTemplateListVO> list(OrderTemplateQuery query);


    OrderTemplateDetailVO getDetail(String id);
}
