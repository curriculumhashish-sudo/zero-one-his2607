package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
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

@Service
public class OrderTemplateServiceImpl
        extends ServiceImpl<OrderTemplateMapper, OrderTemplate>
        implements IOrderTemplateService {

    @Resource
    private OrderTemplateLineMapper orderTemplateLineMapper;

    @Override
    public List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query) {
        LambdaQueryWrapper<OrderTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(OrderTemplate::getId, OrderTemplate::getName,
                       OrderTemplate::getBindingEnum, OrderTemplate::getTemplateKindEnum,
                       OrderTemplate::getSceneEnum)
               .eq(OrderTemplate::getStatusEnum, "ACTIVE")
               .eq(OrderTemplate::getSceneEnum, query.getSceneEnum())
               .eq(OrderTemplate::getTemplateKindEnum, query.getTemplateKindEnum());

        if (query.getPractitionerId() != null && query.getDeptId() != null) {
            wrapper.and(w -> w
                .eq(OrderTemplate::getBindingEnum, "GLOBAL")
                .or().eq(OrderTemplate::getBindingEnum, "DEPT")
                      .eq(OrderTemplate::getObjectId, query.getDeptId())
                .or().eq(OrderTemplate::getBindingEnum, "PERSONAL")
                      .eq(OrderTemplate::getObjectId, query.getPractitionerId()));
        } else {
            wrapper.eq(OrderTemplate::getBindingEnum, "GLOBAL");
        }

        wrapper.orderByAsc(OrderTemplate::getSortWeight, OrderTemplate::getName);

        List<OrderTemplate> entities = baseMapper.selectList(wrapper);
        List<OrderTemplateNameVO> vos = new ArrayList<>(entities.size());
        entities.forEach(e -> vos.add(BeanUtil.copyProperties(e, OrderTemplateNameVO.class)));
        return vos;
    }

    @Override
    public Page<OrderTemplateListVO> list(OrderTemplateQuery query) {
        LambdaQueryWrapper<OrderTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(OrderTemplate::getId, OrderTemplate::getName,
                       OrderTemplate::getTemplateCode, OrderTemplate::getBindingEnum,
                       OrderTemplate::getSceneEnum, OrderTemplate::getTemplateKindEnum,
                       OrderTemplate::getStatusEnum, OrderTemplate::getSortWeight,
                       OrderTemplate::getCreatedAt, OrderTemplate::getUpdatedAt)
               .eq(OrderTemplate::getIsDeleted, false)
               .eq(OrderTemplate::getTemplateKindEnum, query.getTemplateKindEnum());

        if (query.getKeyword() != null && !query.getKeyword().isBlank()) {
            wrapper.like(OrderTemplate::getName, query.getKeyword());
        }
        if (query.getBindingEnum() != null && !query.getBindingEnum().isBlank()) {
            wrapper.eq(OrderTemplate::getBindingEnum, query.getBindingEnum());
        }
        if (query.getSceneEnum() != null && !query.getSceneEnum().isBlank()) {
            wrapper.eq(OrderTemplate::getSceneEnum, query.getSceneEnum());
        }
        if (query.getStatusEnum() != null && !query.getStatusEnum().isBlank()) {
            wrapper.eq(OrderTemplate::getStatusEnum, query.getStatusEnum());
        }

        wrapper.orderByAsc(OrderTemplate::getSortWeight)
               .orderByDesc(OrderTemplate::getUpdatedAt);

        Page<OrderTemplate> entityPage = baseMapper.selectPage(
                new Page<>(query.getPageIndex(), query.getPageSize()), wrapper);

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

        LambdaQueryWrapper<OrderTemplateLine> lineWrapper = new LambdaQueryWrapper<>();
        lineWrapper.select(OrderTemplateLine::getId, OrderTemplateLine::getLineKindEnum,
                           OrderTemplateLine::getMedicationDefinitionId,
                           OrderTemplateLine::getActivityDefinitionId,
                           OrderTemplateLine::getDeviceDefinitionId,
                           OrderTemplateLine::getGroupKey, OrderTemplateLine::getSortOrder,
                           OrderTemplateLine::getDefaultQuantity, OrderTemplateLine::getDefaultUnitCode,
                           OrderTemplateLine::getDefaultDays, OrderTemplateLine::getFrequencyId,
                           OrderTemplateLine::getRouteCode, OrderTemplateLine::getDosageText,
                           OrderTemplateLine::getDefaultPerformOrgId, OrderTemplateLine::getLineRemark,
                           OrderTemplateLine::getChineseHerbsDoseQuantity,
                           OrderTemplateLine::getSufferingFlag)
                   .eq(OrderTemplateLine::getOrderTemplateId, id)
                   .eq(OrderTemplateLine::getIsDeleted, false)
                   .orderByAsc(OrderTemplateLine::getSortOrder, OrderTemplateLine::getId);

        List<OrderTemplateLine> lines = orderTemplateLineMapper.selectList(lineWrapper);
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