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

/**
 * <p>
 * 组套模板 服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
public interface IOrderTemplateService extends IService<OrderTemplate> {

    /**
     * 获取组套名称下拉列表（支持个人/科室/全院可见性过滤）
     * @param query 查询条件
     * @return 名称列表
     */
    List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query);

    /**
     * 获取组套模板分页列表
     * @param query 查询条件
     * @return 分页结果
     */
    Page<OrderTemplateListVO> list(OrderTemplateQuery query);

    /**
     * 获取组套模板详情（含明细行）
     * @param id 组套模板头 ID
     * @return 详情 VO，不存在时返回 null
     */
    OrderTemplateDetailVO getDetail(String id);
}