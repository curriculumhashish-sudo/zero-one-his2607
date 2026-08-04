package com.zeroone.star.setroster.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateItemDTO;
import com.zeroone.star.project.query.j8.setroster.WeekScheduleTemplateQuery;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;
import com.zeroone.star.setroster.convert.WeekScheduleTemplateConvert;
import com.zeroone.star.setroster.entity.WeekScheduleTemplate;
import com.zeroone.star.setroster.entity.WeekScheduleTemplateItem;
import com.zeroone.star.setroster.mapper.WeekScheduleTemplateItemMapper;
import com.zeroone.star.setroster.mapper.WeekScheduleTemplateMapper;
import com.zeroone.star.setroster.service.WeekScheduleTemplateService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：周排班模板 Service 实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Service
public class WeekScheduleTemplateServiceImpl extends ServiceImpl<WeekScheduleTemplateMapper, WeekScheduleTemplate>
        implements WeekScheduleTemplateService {

    @Resource
    private WeekScheduleTemplateMapper weekScheduleTemplateMapper;

    @Resource
    private WeekScheduleTemplateItemMapper weekScheduleTemplateItemMapper;

    @Resource
    private WeekScheduleTemplateConvert convert;

    @Override
    public PageDTO<WeekScheduleTemplateDTO> page(WeekScheduleTemplateQuery query) {
        LambdaQueryWrapper<WeekScheduleTemplate> wrapper = new LambdaQueryWrapper<>();

        if (query.getKeyword() != null && !query.getKeyword().isBlank()) {
            wrapper.and(w -> w
                    .like(WeekScheduleTemplate::getTemplateCode, query.getKeyword())
                    .or()
                    .like(WeekScheduleTemplate::getTemplateName, query.getKeyword())
            );
        }
        if (query.getOrganizationId() != null && !query.getOrganizationId().isBlank()) {
            wrapper.eq(WeekScheduleTemplate::getOrganizationId, query.getOrganizationId());
        }
        if (query.getEnabled() != null) {
            wrapper.eq(WeekScheduleTemplate::getEnabled, query.getEnabled());
        }

        wrapper.orderByDesc(WeekScheduleTemplate::getCreatedAt);

        Page<WeekScheduleTemplate> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<WeekScheduleTemplate> result = weekScheduleTemplateMapper.selectPage(page, wrapper);

        List<WeekScheduleTemplateDTO> dtoList = result.getRecords().stream()
                .map(convert::toDTO)
                .collect(Collectors.toList());

        return new PageDTO<>(result.getCurrent(), result.getSize(), result.getTotal(), result.getPages(), dtoList);
    }

    @Override
    public WeekScheduleTemplateDetailVO getDetail(String id) {
        WeekScheduleTemplate entity = weekScheduleTemplateMapper.selectById(id);
        if (entity == null) {
            return null;
        }

        // 查询明细
        LambdaQueryWrapper<WeekScheduleTemplateItem> itemWrapper = new LambdaQueryWrapper<>();
        itemWrapper.eq(WeekScheduleTemplateItem::getWeekScheduleTemplateId, id)
                .orderByAsc(WeekScheduleTemplateItem::getSortNo);
        List<WeekScheduleTemplateItem> items = weekScheduleTemplateItemMapper.selectList(itemWrapper);

        WeekScheduleTemplateDetailVO vo = convert.toDetailVO(entity);
        vo.setItems(items.stream().map(convert::toDetailItemVO).collect(Collectors.toList()));

        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String add(AddWeekScheduleTemplateDTO dto) {
        // 校验 templateCode 唯一性
        LambdaQueryWrapper<WeekScheduleTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WeekScheduleTemplate::getTemplateCode, dto.getTemplateCode());
        if (dto.getOrganizationId() != null) {
            wrapper.eq(WeekScheduleTemplate::getOrganizationId, dto.getOrganizationId());
        } else {
            wrapper.isNull(WeekScheduleTemplate::getOrganizationId);
        }
        Long count = weekScheduleTemplateMapper.selectCount(wrapper);
        if (count > 0) {
            return null;
        }

        // 插入主表
        WeekScheduleTemplate entity = convert.toEntity(dto);
        weekScheduleTemplateMapper.insert(entity);

        // 插入明细
        List<WeekScheduleTemplateItemDTO> dtoItems = dto.getItems();
        for (WeekScheduleTemplateItemDTO itemDTO : dtoItems) {
            WeekScheduleTemplateItem item = convert.toItemEntity(itemDTO);
            item.setWeekScheduleTemplateId(entity.getId());
            weekScheduleTemplateItemMapper.insert(item);
        }

        return entity.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String edit(WeekScheduleTemplateDTO dto) {
        // 检查模板是否存在
        WeekScheduleTemplate exist = weekScheduleTemplateMapper.selectById(dto.getId());
        if (exist == null) {
            return null;
        }

        // 校验 templateCode 唯一性（排除自身）
        LambdaQueryWrapper<WeekScheduleTemplate> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WeekScheduleTemplate::getTemplateCode, dto.getTemplateCode())
                .ne(WeekScheduleTemplate::getId, dto.getId());
        if (dto.getOrganizationId() != null) {
            wrapper.eq(WeekScheduleTemplate::getOrganizationId, dto.getOrganizationId());
        } else {
            wrapper.isNull(WeekScheduleTemplate::getOrganizationId);
        }
        Long count = weekScheduleTemplateMapper.selectCount(wrapper);
        if (count > 0) {
            return null;
        }

        // 更新主表
        WeekScheduleTemplate entity = convert.toEntity(dto);
        weekScheduleTemplateMapper.updateById(entity);

        // 处理明细（方案 B：逐条对比）
        List<WeekScheduleTemplateItemDTO> dtoItems = dto.getItems();

        // 收集前端传入的id（过滤null）
        List<String> inputIds = dtoItems.stream()
                .map(WeekScheduleTemplateItemDTO::getId)
                .filter(Objects::nonNull)
                .collect(Collectors.toList());

        // 逐条处理
        for (WeekScheduleTemplateItemDTO itemDTO : dtoItems) {
            WeekScheduleTemplateItem item = convert.toItemEntity(itemDTO);
            if (item.getId() != null) {
                // 有id → 更新
                weekScheduleTemplateItemMapper.updateById(item);
            } else {
                // 无id → 新增
                item.setWeekScheduleTemplateId(dto.getId());
                weekScheduleTemplateItemMapper.insert(item);
            }
        }

        // 删除前端没传的明细
        if (!inputIds.isEmpty()) {
            weekScheduleTemplateItemMapper.delete(
                    new LambdaQueryWrapper<WeekScheduleTemplateItem>()
                            .eq(WeekScheduleTemplateItem::getWeekScheduleTemplateId, dto.getId())
                            .notIn(WeekScheduleTemplateItem::getId, inputIds)
            );
        } else {
            // 前端传了空列表，删除所有明细
            weekScheduleTemplateItemMapper.delete(
                    new LambdaQueryWrapper<WeekScheduleTemplateItem>()
                            .eq(WeekScheduleTemplateItem::getWeekScheduleTemplateId, dto.getId())
            );
        }

        return dto.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public boolean delete(String id) {
        WeekScheduleTemplate exist = weekScheduleTemplateMapper.selectById(id);
        if (exist == null) {
            return false;
        }

        // 删除明细
        weekScheduleTemplateItemMapper.delete(
                new LambdaQueryWrapper<WeekScheduleTemplateItem>()
                        .eq(WeekScheduleTemplateItem::getWeekScheduleTemplateId, id)
        );

        // 软删除主表
        weekScheduleTemplateMapper.deleteById(id);

        return true;
    }
}
