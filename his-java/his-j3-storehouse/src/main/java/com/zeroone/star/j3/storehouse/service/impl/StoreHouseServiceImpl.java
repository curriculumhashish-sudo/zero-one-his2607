package com.zeroone.star.j3.storehouse.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.j3.storehouse.entity.DepartmentDO;
import com.zeroone.star.j3.storehouse.entity.StoreHouseDO;
import com.zeroone.star.j3.storehouse.mapper.DepartmentMapper;
import com.zeroone.star.j3.storehouse.mapper.StoreHouseMapper;
import com.zeroone.star.j3.storehouse.service.StoreHouseService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseAddDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseUpdateDTO;
import com.zeroone.star.project.enums.j3.storehouse.*;
import com.zeroone.star.project.exception.j3.storehouse.StoreHouseNotFoundException;
import com.zeroone.star.project.query.j3.storehouse.StoreHouseQuery;
import com.zeroone.star.project.vo.j3.storehouse.DepartmentSimpleVO;
import com.zeroone.star.project.vo.j3.storehouse.StoreHouseVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

/**
 * 库房药房服务实现
 * @author mie
 * @version 1.0.0
 */
@Service
@Slf4j
public class StoreHouseServiceImpl implements StoreHouseService {

    @Resource
    private StoreHouseMapper storeHouseMapper;

    @Resource
    private DepartmentMapper departmentMapper;

    private static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    /** 库房药房模块管理的 formEnum 值 */
    private static final List<String> STOREHOUSE_FORMS = FormEnum.STOREHOUSE_FORMS;

    @Override
    public List<StoreHouseVO> getStoreHouseNames(String formEnum) {
        LambdaQueryWrapper<StoreHouseDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS);
        if (formEnum != null && !formEnum.isBlank()) {
            wrapper.eq(StoreHouseDO::getFormEnum, formEnum);
        }
        wrapper.orderByAsc(StoreHouseDO::getDisplayOrder)
               .orderByAsc(StoreHouseDO::getCreatedAt);
        List<StoreHouseDO> list = storeHouseMapper.selectList(wrapper);
        Map<Long, String> parentNameMap = buildParentNameMap(list);
        return list.stream().map(e -> convertToVO(e, parentNameMap)).collect(Collectors.toList());
    }

    @Override
    public PageDTO<StoreHouseVO> list(StoreHouseQuery query) {
        LambdaQueryWrapper<StoreHouseDO> wrapper = new LambdaQueryWrapper<>();
        // 只查库房药房类型
        wrapper.in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS);
        // 关键字模糊搜索（名称/拼音码）
        if (query.getKeyword() != null && !query.getKeyword().isBlank()) {
            wrapper.and(w -> w
                    .like(StoreHouseDO::getName, query.getKeyword())
                    .or()
                    .like(StoreHouseDO::getPyStr, query.getKeyword()));
        }
        wrapper.orderByAsc(StoreHouseDO::getDisplayOrder)
               .orderByAsc(StoreHouseDO::getCreatedAt);

        Page<StoreHouseDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<StoreHouseDO> result = storeHouseMapper.selectPage(page, wrapper);

        Map<Long, String> parentNameMap = buildParentNameMap(result.getRecords());
        return PageDTO.create(result, e -> convertToVO(e, parentNameMap));
    }

    @Override
    @Transactional
    public void add(StoreHouseAddDTO dto) {
        StoreHouseDO entity = new StoreHouseDO();
        entity.setName(dto.getName());
        entity.setFormEnum(dto.getFormEnum());
        entity.setDisplayOrder(0);
        // 默认启用
        entity.setStatusEnum(StorehouseStatusEnum.ACTIVE.getDesc());
        // 默认模式为"具体实例"
        entity.setModeEnum(ModeEnum.INSTANCE.getDesc());
        entity.setOrganizationId(dto.getOrganizationId() != null ? dto.getOrganizationId() : 322663105141149696L);
        // 生成业务编号
        entity.setBusNo(generateBusNo());
        entity.setCreatedAt(LocalDateTime.now());
        entity.setUpdatedAt(LocalDateTime.now());

        storeHouseMapper.insert(entity);
    }

    @Override
    @Transactional
    public void update(StoreHouseUpdateDTO dto) {
        StoreHouseDO existing = storeHouseMapper.selectById(dto.getId());
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + dto.getId());
        }

        LambdaUpdateWrapper<StoreHouseDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(StoreHouseDO::getId, dto.getId())
               .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS)
               .set(StoreHouseDO::getName, dto.getName())
               .set(StoreHouseDO::getFormEnum, dto.getFormEnum())
               .set(StoreHouseDO::getOperationalEnum, dto.getOperationalEnum())
               .set(StoreHouseDO::getModeEnum, dto.getModeEnum())
               .set(StoreHouseDO::getOrganizationId, dto.getOrganizationId())
               .set(StoreHouseDO::getDisplayOrder, dto.getDisplayOrder())
               .set(StoreHouseDO::getPyStr, dto.getPyStr())
               .set(StoreHouseDO::getWbStr, dto.getWbStr())
               .set(StoreHouseDO::getParentId, dto.getParentId())
               .set(StoreHouseDO::getUpdatedAt, LocalDateTime.now());

        storeHouseMapper.update(wrapper);
    }

    @Override
    @Transactional
    public void delete(Long id) {
        StoreHouseDO existing = storeHouseMapper.selectById(id);
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + id);
        }
        storeHouseMapper.deleteById(id);
    }

    @Override
    @Transactional
    public void toggleEnabled(Long id, Byte isEnabled) {
        StoreHouseDO existing = storeHouseMapper.selectById(id);
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + id);
        }

        String statusEnum = isEnabled == 1
                ? StorehouseStatusEnum.ACTIVE.getDesc()
                : StorehouseStatusEnum.INACTIVE.getDesc();

        LambdaUpdateWrapper<StoreHouseDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(StoreHouseDO::getId, id)
               .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS)
               .set(StoreHouseDO::getStatusEnum, statusEnum)
               .set(StoreHouseDO::getUpdatedAt, LocalDateTime.now());

        storeHouseMapper.update(wrapper);
    }

    /**
     * DO 转 VO
     */
    private StoreHouseVO convertToVO(StoreHouseDO entity, Map<Long, String> parentNameMap) {
        if (entity == null) return null;
        StoreHouseVO vo = new StoreHouseVO();
        vo.setId(entity.getId() != null ? entity.getId().toString() : null);
        vo.setBusNo(entity.getBusNo());
        vo.setName(entity.getName());
        vo.setStatusEnum(entity.getStatusEnum());
        vo.setStatusEnumText(StorehouseStatusEnum.getDictTextByDesc(entity.getStatusEnum()));
        vo.setOperationalEnum(entity.getOperationalEnum());
        vo.setOperationalEnumText(OperationalEnum.getDictTextByDesc(entity.getOperationalEnum()));
        vo.setModeEnum(entity.getModeEnum());
        vo.setModeEnumText(ModeEnum.getDictTextByDesc(entity.getModeEnum()));
        vo.setFormEnum(entity.getFormEnum());
        vo.setFormEnumText(FormEnum.getDictTextByDesc(entity.getFormEnum()));
        vo.setOrganizationId(entity.getOrganizationId() != null ? entity.getOrganizationId().toString() : null);
        vo.setPyStr(entity.getPyStr());
        vo.setWbStr(entity.getWbStr());
        vo.setDisplayOrder(entity.getDisplayOrder());
        vo.setParentId(entity.getParentId() != null ? entity.getParentId().toString() : null);
        if (entity.getParentId() != null && parentNameMap != null) {
            vo.setParentName(parentNameMap.getOrDefault(entity.getParentId(), null));
        }
        // 解析 typeJson
        vo.setTypeExtensions(parseTypeJson(entity.getTypeJson()));
        return vo;
    }

    /**
     * 批量查询上级科室名称
     */
    private Map<Long, String> buildParentNameMap(List<StoreHouseDO> list) {
        Set<Long> parentIds = list.stream()
                .map(StoreHouseDO::getParentId)
                .filter(Objects::nonNull)
                .collect(Collectors.toSet());
        if (parentIds.isEmpty()) {
            return Collections.emptyMap();
        }
        List<DepartmentDO> departments = departmentMapper.selectBatchIds(parentIds);
        return departments.stream()
                .collect(Collectors.toMap(DepartmentDO::getId, DepartmentDO::getName));
    }

    @Override
    public List<DepartmentSimpleVO> listParentDepartments() {
        LambdaQueryWrapper<DepartmentDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(DepartmentDO::getTypeEnum, "DEPARTMENT")
               .eq(DepartmentDO::getStatusEnum, "ACTIVE")
               .eq(DepartmentDO::getActiveFlag, true);
        List<DepartmentDO> list = departmentMapper.selectList(wrapper);
        return list.stream()
                .map(d -> new DepartmentSimpleVO(d.getId().toString(), d.getName()))
                .collect(Collectors.toList());
    }

    /**
     * 解析 typeJson 字段为 Map
     */
    @SuppressWarnings("unchecked")
    private Map<String, Object> parseTypeJson(String typeJson) {
        if (typeJson == null || typeJson.isBlank()) {
            return new HashMap<>();
        }
        try {
            return OBJECT_MAPPER.readValue(typeJson, Map.class);
        } catch (Exception e) {
            log.warn("解析 typeJson 失败: {}", typeJson, e);
            return new HashMap<>();
        }
    }

    /**
     * 生成业务编号 (LO + 4位序号)，使用 FOR UPDATE 行锁防并发
     */
    private String generateBusNo() {
        String maxBusNo = storeHouseMapper.selectMaxBusNoForUpdate();
        int nextNum = 1;
        if (maxBusNo != null && maxBusNo.startsWith("LO")) {
            try {
                nextNum = Integer.parseInt(maxBusNo.substring(2)) + 1;
            } catch (NumberFormatException e) {
                log.warn("解析 busNo 失败: {}", maxBusNo);
            }
        }
        return String.format("LO%04d", nextNum);
    }
}
