package com.zeroone.star.j3.storehouse.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.j3.storehouse.entity.DepartmentDO;
import com.zeroone.star.j3.storehouse.entity.StoreHouseDO;
import com.zeroone.star.j3.storehouse.mapper.DepartmentMapper;
import com.zeroone.star.j3.storehouse.mapper.StoreHouseMapper;
import com.zeroone.star.j3.storehouse.service.StoreHouseService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseAddDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseUpdateDTO;
import com.zeroone.star.project.enums.j3.storehouse.FormEnum;
import com.zeroone.star.project.enums.j3.storehouse.ModeEnum;
import com.zeroone.star.project.enums.j3.storehouse.OperationalEnum;
import com.zeroone.star.project.enums.j3.storehouse.StorehouseStatusEnum;
import com.zeroone.star.project.exception.j3.storehouse.StoreHouseDuplicateException;
import com.zeroone.star.project.exception.j3.storehouse.StoreHouseNotFoundException;
import com.zeroone.star.project.exception.j3.storehouse.StoreHouseTenantAccessException;
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
 *
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

    @Resource
    private UserHolder userHolder;

    private static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();

    /**
     * 库房药房模块管理的 formEnum 值
     */
    private static final List<String> STOREHOUSE_FORMS = FormEnum.STOREHOUSE_FORMS;

    @Override
    public List<StoreHouseVO> getStoreHouseNames(String formEnum) {
        String tenantId = resolveCurrentTenantId();
        // 非空时校验 formEnum 必须属于库房药房允许范围
        if (formEnum != null && !formEnum.isBlank()) {
            validateFormEnum(formEnum);
        }
        LambdaQueryWrapper<StoreHouseDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(StoreHouseDO::getTenantId, tenantId)
                .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS);
        if (formEnum != null && !formEnum.isBlank()) {
            wrapper.eq(StoreHouseDO::getFormEnum, formEnum);
        }
        wrapper.orderByAsc(StoreHouseDO::getDisplayOrder)
                .orderByAsc(StoreHouseDO::getCreatedAt);
        List<StoreHouseDO> list = storeHouseMapper.selectList(wrapper);
        Map<String, String> parentNameMap = buildParentNameMap(list);
        return list.stream().map(e -> convertToVO(e, parentNameMap)).collect(Collectors.toList());
    }

    @Override
    public PageDTO<StoreHouseVO> list(StoreHouseQuery query) {
        String tenantId = resolveCurrentTenantId();
        LambdaQueryWrapper<StoreHouseDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(StoreHouseDO::getTenantId, tenantId)
                .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS);
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

        Map<String, String> parentNameMap = buildParentNameMap(result.getRecords());
        return PageDTO.create(result, e -> convertToVO(e, parentNameMap));
    }

    @Override
    @Transactional
    public void add(StoreHouseAddDTO dto) {
        String tenantId = resolveCurrentTenantId();
        // 校验 formEnum 必须属于库房药房允许范围
        validateFormEnum(dto.getFormEnum());
        // 校验 organizationId 是否真实存在、属于当前租户、为有效机构
        validateOrganizationId(dto.getOrganizationId(), tenantId);

        LambdaQueryWrapper<StoreHouseDO> dupCheck = new LambdaQueryWrapper<>();
        dupCheck.eq(StoreHouseDO::getTenantId, tenantId)
                .eq(StoreHouseDO::getName, dto.getName())
                .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS);
        if (storeHouseMapper.selectCount(dupCheck) > 0) {
            throw new StoreHouseDuplicateException("当前租户下已存在同名库房/药房：" + dto.getName());
        }

        StoreHouseDO entity = new StoreHouseDO();
        entity.setTenantId(tenantId);
        entity.setName(dto.getName());
        entity.setFormEnum(dto.getFormEnum());
        entity.setDisplayOrder(0);
        entity.setStatusEnum(StorehouseStatusEnum.ACTIVE.getDesc());
        entity.setModeEnum(ModeEnum.INSTANCE.getDesc());
        entity.setOrganizationId(dto.getOrganizationId());
        entity.setBusNo(generateBusNo());
        entity.setCreatedAt(LocalDateTime.now());
        entity.setUpdatedAt(LocalDateTime.now());

        storeHouseMapper.insert(entity);
    }

    @Override
    @Transactional
    public void update(StoreHouseUpdateDTO dto) {
        String tenantId = resolveCurrentTenantId();
        // 校验 formEnum 必须属于库房药房允许范围
        validateFormEnum(dto.getFormEnum());
        // 校验 organizationId 是否真实存在、属于当前租户、为有效机构
        validateOrganizationId(dto.getOrganizationId(), tenantId);

        StoreHouseDO existing = storeHouseMapper.selectById(dto.getId());
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + dto.getId());
        }
        if (!Objects.equals(existing.getTenantId(), tenantId)) {
            throw new StoreHouseTenantAccessException("无权修改其他租户的库房药房数据");
        }

        LambdaUpdateWrapper<StoreHouseDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(StoreHouseDO::getId, dto.getId())
                .eq(StoreHouseDO::getTenantId, tenantId)
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
    public void delete(String id) {
        String tenantId = resolveCurrentTenantId();
        StoreHouseDO existing = storeHouseMapper.selectById(id);
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + id);
        }
        if (!Objects.equals(existing.getTenantId(), tenantId)) {
            throw new StoreHouseTenantAccessException("无权删除其他租户的库房药房数据");
        }
        storeHouseMapper.deleteById(id);
    }

    @Override
    @Transactional
    public void toggleEnabled(String id, Byte isEnabled) {
        if (isEnabled == null || (isEnabled != 0 && isEnabled != 1)) {
            throw new IllegalArgumentException("isEnabled 参数非法，仅允许 0(停用) 或 1(启用)，当前值: " + isEnabled);
        }

        String tenantId = resolveCurrentTenantId();
        StoreHouseDO existing = storeHouseMapper.selectById(id);
        if (existing == null || !STOREHOUSE_FORMS.contains(existing.getFormEnum())) {
            throw new StoreHouseNotFoundException("库房药房不存在，ID: " + id);
        }
        if (!Objects.equals(existing.getTenantId(), tenantId)) {
            throw new StoreHouseTenantAccessException("无权操作其他租户的库房药房数据");
        }

        // 停用时校验业务引用
        if (isEnabled == 0) {
            checkBusinessReferenceBeforeDisable(existing, tenantId);
        }

        String statusEnum = isEnabled == 1
                ? StorehouseStatusEnum.ACTIVE.getDesc()
                : StorehouseStatusEnum.INACTIVE.getDesc();

        LambdaUpdateWrapper<StoreHouseDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(StoreHouseDO::getId, id)
                .eq(StoreHouseDO::getTenantId, tenantId)
                .in(StoreHouseDO::getFormEnum, STOREHOUSE_FORMS)
                .set(StoreHouseDO::getStatusEnum, statusEnum)
                .set(StoreHouseDO::getUpdatedAt, LocalDateTime.now());

        storeHouseMapper.update(wrapper);
    }

    /**
     * DO 转 VO
     */
    private StoreHouseVO convertToVO(StoreHouseDO entity, Map<String, String> parentNameMap) {
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
    private Map<String, String> buildParentNameMap(List<StoreHouseDO> list) {
        Set<String> parentIds = list.stream()
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
        String tenantId = resolveCurrentTenantId();
        LambdaQueryWrapper<DepartmentDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(DepartmentDO::getTenantId, tenantId)
                .eq(DepartmentDO::getTypeEnum, "DEPARTMENT")
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
     * 校验 formEnum 必须属于库房药房允许范围（WAREHOUSE/PHARMACY/DEPT_PHARMACY）
     */
    private void validateFormEnum(String formEnum) {
        if (formEnum == null || formEnum.isBlank()) {
            throw new IllegalArgumentException("形态枚举(formEnum)不能为空");
        }
        if (!STOREHOUSE_FORMS.contains(formEnum)) {
            throw new IllegalArgumentException(
                    "形态枚举(formEnum)不合法，仅允许: " + String.join(" / ", STOREHOUSE_FORMS) + "，当前值: " + formEnum);
        }
    }

    /**
     * 校验 organizationId 是否真实存在、是否属于当前租户、是否为有效机构
     */
    private void validateOrganizationId(String organizationId, String tenantId) {
        if (organizationId == null) {
            return;
        }
        DepartmentDO org = departmentMapper.selectById(organizationId);
        if (org == null) {
            throw new IllegalArgumentException("机构不存在，organizationId: " + organizationId);
        }
        if (!Objects.equals(org.getTenantId(), tenantId)) {
            throw new StoreHouseTenantAccessException("所选机构不属于当前租户，无权关联");
        }
        if (!"ACTIVE".equals(org.getStatusEnum()) || !Boolean.TRUE.equals(org.getActiveFlag())) {
            throw new IllegalArgumentException("所选机构已停用，不能关联到库房药房，organizationId: " + organizationId);
        }
    }

    /**
     * 停用前业务引用校验：检查子节点、库存、入出库单据、发药记录等业务引用
     */
    private void checkBusinessReferenceBeforeDisable(StoreHouseDO storeHouse, String tenantId) {
        String locationId = storeHouse.getId().toString();

        // 1. 检查是否存在启用中的子节点
        LambdaQueryWrapper<StoreHouseDO> childCheck = new LambdaQueryWrapper<>();
        childCheck.eq(StoreHouseDO::getParentId, storeHouse.getId())
                .eq(StoreHouseDO::getTenantId, tenantId)
                .eq(StoreHouseDO::getStatusEnum, StorehouseStatusEnum.ACTIVE.getDesc());
        if (storeHouseMapper.selectCount(childCheck) > 0) {
            throw new IllegalArgumentException("该库房药房下存在启用中的子节点，不能停用");
        }

        // 2. 检查是否仍有库存（数量>0）
        long inventoryCount = storeHouseMapper.countInventoryByLocationId(locationId);
        if (inventoryCount > 0) {
            throw new IllegalArgumentException("该库房药房存在库存记录（" + inventoryCount + "条），不能停用，请先完成库存清零或转移");
        }

        // 3. 检查是否存在未完成的入出库/供应单据
        long supplyCount = storeHouseMapper.countUnfinishedSupplyRequestByLocationId(locationId);
        if (supplyCount > 0) {
            throw new IllegalArgumentException("该库房药房存在未完成的入出库单据（" + supplyCount + "条），不能停用，请先完成或取消相关单据");
        }

        // 4. 检查是否存在未完成的发药记录
        long dispenseCount = storeHouseMapper.countUnfinishedDispenseByLocationId(locationId);
        if (dispenseCount > 0) {
            throw new IllegalArgumentException("该库房药房存在未完成的发药记录（" + dispenseCount + "条），不能停用，请先完成或取消相关发药");
        }
    }

    /**
     * 获取当前登录用户的租户ID，获取失败抛出越权异常
     */
    private String resolveCurrentTenantId() {
        try {
            String tenantIdStr = userHolder.getCurrentTenantId();
            if (StrUtil.isBlank(tenantIdStr)) {
                throw new StoreHouseTenantAccessException("无法获取当前用户的租户信息，请重新登录");
            }
            return tenantIdStr.trim();
        } catch (StoreHouseTenantAccessException e) {
            throw e;
        } catch (Exception e) {
            log.error("获取当前租户ID失败", e);
            throw new StoreHouseTenantAccessException("无法获取当前用户的租户信息，请重新登录");
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
