package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.DeptStockDO;
import com.zeroone.star.configuration.mapper.DeptStockMapper;
import com.zeroone.star.configuration.service.DeptStockService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.deptstock.DeptStockSaveDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.query.j5.deptstock.DeptStockQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.deptstock.DeptStockVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * 配置管理-取药科室库存服务实现（映射wkf_inventory_item表）
 */
@Service
@RequiredArgsConstructor
public class DeptStockServiceImpl implements DeptStockService {

    private final DeptStockMapper deptStockMapper;
    private final UserHolder userHolder;

    @Override
    public JsonVO<PageDTO<DeptStockVO>> listDeptStock(DeptStockQuery query) {
        Page<DeptStockDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        LambdaQueryWrapper<DeptStockDO> wrapper = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getCategoryCode())) {
            wrapper.eq(DeptStockDO::getCategoryCode, query.getCategoryCode());
        }
        if (StringUtils.hasText(query.getItemTable())) {
            wrapper.eq(DeptStockDO::getItemTable, query.getItemTable());
        }
        if (StringUtils.hasText(query.getName())) {
            wrapper.like(DeptStockDO::getName, query.getName());
        }
        if (StringUtils.hasText(query.getItemId())) {
            wrapper.eq(DeptStockDO::getItemId, query.getItemId());
        }
        if (StringUtils.hasText(query.getInventoryStatusEnum())) {
            wrapper.eq(DeptStockDO::getInventoryStatusEnum, query.getInventoryStatusEnum());
        }
        if (StringUtils.hasText(query.getLocationId())) {
            wrapper.eq(DeptStockDO::getLocationId, query.getLocationId());
        }
        wrapper.orderByDesc(DeptStockDO::getUpdatedAt);

        Page<DeptStockDO> result = deptStockMapper.selectPage(page, wrapper);
        List<DeptStockVO> voList = new ArrayList<>();
        for (DeptStockDO record : result.getRecords()) {
            DeptStockVO vo = new DeptStockVO();
            BeanUtils.copyProperties(record, vo);
            voList.add(vo);
        }

        PageDTO<DeptStockVO> pageDTO = new PageDTO<>();
        pageDTO.setRows(voList);
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setPages(result.getPages());

        return JsonVO.success(pageDTO);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> saveDeptStock(DeptStockSaveDTO dto) {
        if (dto.getItems() == null || dto.getItems().isEmpty()) {
            throw new BusinessException("保存项目不能为空");
        }

        int savedCount = 0;
        for (DeptStockSaveDTO.DeptStockItemDTO item : dto.getItems()) {
            DeptStockDO entity;

            if (StringUtils.hasText(item.getId())) {
                entity = deptStockMapper.selectById(item.getId());
                if (entity == null) {
                    throw new BusinessException("记录不存在: " + item.getId());
                }
            } else {
                entity = new DeptStockDO();
                fillAuditFields(entity, true);
            }

            BeanUtils.copyProperties(item, entity);
            fillAuditFields(entity, !StringUtils.hasText(item.getId()));

            if (StringUtils.hasText(item.getId())) {
                deptStockMapper.updateById(entity);
            } else {
                deptStockMapper.insert(entity);
            }
            savedCount++;
        }

        return JsonVO.success("保存成功，共处理" + savedCount + "条");
    }

    private void fillAuditFields(DeptStockDO entity, boolean isInsert) {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setTenantId(userHolder.getCurrentTenantId());
                if (isInsert) {
                    entity.setCreatedBy(currentUser.getId());
                }
                entity.setUpdatedBy(currentUser.getId());
            }
        } catch (Exception ignored) {
        }
    }
}
