package com.zeroone.star.hisj5basedata.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.hisj5basedata.entity.OrderTemplate;
import com.zeroone.star.hisj5basedata.entity.OrderTemplateLine;
import com.zeroone.star.hisj5basedata.mapper.OrderTemplateLineMapper;
import com.zeroone.star.hisj5basedata.mapper.OrderTemplateMapper;
import com.zeroone.star.hisj5basedata.query.OrderTemplateNameQuery;
import com.zeroone.star.hisj5basedata.query.OrderTemplateQuery;
import com.zeroone.star.hisj5basedata.service.IOrderTemplateService;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateDetailVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateLineVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateListVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateNameVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 组套模板 服务实现类
 * </p>
 * @author keepasdd
 */
@Service
public class OrderTemplateServiceImpl
        extends ServiceImpl<OrderTemplateMapper, OrderTemplate>
        implements IOrderTemplateService {

    @Resource
    private OrderTemplateLineMapper orderTemplateLineMapper;

    @Override
    public List<OrderTemplateNameVO> listNames(OrderTemplateNameQuery query) {
        LambdaQueryWrapper<OrderTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(OrderTemplate::getStatusEnum, "ACTIVE")
               .eq(OrderTemplate::getSceneEnum, query.getSceneEnum())
               .eq(OrderTemplate::getTemplateKindEnum, query.getTemplateKindEnum());

        // 全院组套始终可见 + 科室/个人按 object_id 匹配
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

        wrapper.select(OrderTemplate::getId, OrderTemplate::getName,
                       OrderTemplate::getBindingEnum, OrderTemplate::getTemplateKindEnum,
                       OrderTemplate::getSceneEnum);

        List<OrderTemplate> entities = baseMapper.selectList(wrapper);

        List<OrderTemplateNameVO> vos = new ArrayList<>();
        entities.forEach(e -> vos.add(BeanUtil.copyProperties(e, OrderTemplateNameVO.class)));
        return vos;
    }

    @Override
    public Page<OrderTemplateListVO> list(OrderTemplateQuery query) {
        LambdaQueryWrapper<OrderTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(OrderTemplate::getTemplateKindEnum, query.getTemplateKindEnum());

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

        wrapper.eq(OrderTemplate::getIsDeleted, false);
        wrapper.orderByAsc(OrderTemplate::getSortWeight);

        Page<OrderTemplate> entityPage = baseMapper.selectPage(
                new Page<>(query.getPageIndex(), query.getPageSize()), wrapper);

        List<OrderTemplateListVO> vos = new ArrayList<>();
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
        if (header == null) {
            return null;
        }

        OrderTemplateDetailVO vo = new OrderTemplateDetailVO();
        BeanUtil.copyProperties(header, vo);

        List<OrderTemplateLine> lines = orderTemplateLineMapper.selectByTemplateId(id);
        List<OrderTemplateLineVO> itemVOs = new ArrayList<>();
        lines.forEach(line -> {
            OrderTemplateLineVO lineVO = new OrderTemplateLineVO();
            BeanUtil.copyProperties(line, lineVO);
            itemVOs.add(lineVO);
        });

        vo.setItems(itemVOs);
        return vo;
    }
}
