package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.OrderTemplate;
import com.zeroone.star.configuration.entity.OrderTemplateLine;
import com.zeroone.star.configuration.mapper.OrderTemplateLineMapper;
import com.zeroone.star.configuration.mapper.OrderTemplateMapper;
import com.zeroone.star.configuration.query.OrderTemplateNameQuery;
import com.zeroone.star.configuration.query.OrderTemplateQuery;
import com.zeroone.star.configuration.service.IOrderTemplateService;
import com.zeroone.star.configuration.vo.OrderTemplateDetailVO;
import com.zeroone.star.configuration.vo.OrderTemplateLineVO;
import com.zeroone.star.configuration.vo.OrderTemplateListVO;
import com.zeroone.star.configuration.vo.OrderTemplateNameVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 组套模板 服务实现类 —— 全部使用显式 SQL 列，禁用 SELECT *
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Service
public class OrderTemplateServiceImpl
        extends ServiceImpl<OrderTemplateMapper, OrderTemplate>
        implements IOrderTemplateService {

    @Resource
    private OrderTemplateLineMapper orderTemplateLineMapper;

    @Override
    public List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query) {
        // 使用显式 SQL 查询，仅返回 nameColumns 指定的 5 列
        List<OrderTemplate> entities = baseMapper.selectNames(
                "ACTIVE",
                query.getSceneEnum(),
                query.getTemplateKindEnum(),
                query.getPractitionerId(),
                query.getDeptId());

        List<OrderTemplateNameVO> vos = new ArrayList<>(entities.size());
        entities.forEach(e -> vos.add(BeanUtil.copyProperties(e, OrderTemplateNameVO.class)));
        return vos;
    }

    @Override
    public Page<OrderTemplateListVO> list(OrderTemplateQuery query) {
        Page<OrderTemplate> entityPage = baseMapper.selectPageExplicit(
                new Page<>(query.getPageIndex(), query.getPageSize()),
                query.getTemplateKindEnum(),
                query.getKeyword(),
                query.getBindingEnum(),
                query.getSceneEnum(),
                query.getStatusEnum());

        List<OrderTemplateListVO> vos = new ArrayList<>(entityPage.getRecords().size());
        entityPage.getRecords().forEach(e -> {
            OrderTemplateListVO vo = new OrderTemplateListVO();
            BeanUtil.copyProperties(e, vo);
            vos.add(vo);
        });

        Page<OrderTemplateListVO> voPage = new Page<>(
                entityPage.getCurrent(), entityPage.getSize(), entityPage.getTotal());
        voPage.setRecords(vos);
        return voPage;
    }

    @Override
    public OrderTemplateDetailVO getDetail(String id) {
        OrderTemplate header = baseMapper.selectById(id);
        if (header == null || Boolean.TRUE.equals(header.getIsDeleted())) {
            return null;
        }

        OrderTemplateDetailVO vo = new OrderTemplateDetailVO();
        BeanUtil.copyProperties(header, vo);

        // 使用显式 SQL 查询明细行，排除软删行
        List<OrderTemplateLine> lines = orderTemplateLineMapper.selectByTemplateId(id);
        List<OrderTemplateLineVO> itemVOs = new ArrayList<>(lines.size());
        lines.forEach(line -> {
            OrderTemplateLineVO lineVO = new OrderTemplateLineVO();
            BeanUtil.copyProperties(line, lineVO);
            itemVOs.add(lineVO);
        });

        vo.setItems(itemVOs);
        return vo;
    }
}