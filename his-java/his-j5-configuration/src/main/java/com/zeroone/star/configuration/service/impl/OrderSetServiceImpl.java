package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.toolkit.Wrappers;

import com.zeroone.star.configuration.converter.OrderSetConverter;
import com.zeroone.star.configuration.entity.OrderSetDO;
import com.zeroone.star.configuration.entity.OrderSetLineDO;
import com.zeroone.star.configuration.enums.*;
import com.zeroone.star.configuration.mapper.OrderSetLineMapper;
import com.zeroone.star.configuration.mapper.OrderSetMapper;
import com.zeroone.star.configuration.service.IOrderSetService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetLineDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.query.j5.configuration.orderset.OrderSetQuery;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetDetailVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetListVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetNameVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * 组套模板写操作实现。
 */
@Service
@RequiredArgsConstructor
public class OrderSetServiceImpl implements IOrderSetService {

    private static final String CONCURRENT_MESSAGE = "组套模板已被其他人修改，请刷新后重试";

    private final OrderSetMapper orderSetMapper;
    private final OrderSetLineMapper orderSetLineMapper;
    private final UserHolder userHolder;

    @Override
    public List<OrderSetNameVO> listOrderSetNames() {
        return orderSetMapper.selectList(Wrappers.<OrderSetDO>lambdaQuery()
                        .eq(OrderSetDO::getIsDeleted, false)
                        .eq(OrderSetDO::getStatusEnum, OrderSetStatusEnum.ACTIVE.name()))
                .stream()
                .map(this::convertToNameVO)
                .toList();
    }

    @Override
    public PageDTO<OrderSetListVO> listOrderSets(OrderSetQuery query) {
        com.baomidou.mybatisplus.extension.plugins.pagination.Page<OrderSetDO> page =
                new com.baomidou.mybatisplus.extension.plugins.pagination.Page<>(query.getPageIndex(), query.getPageSize());

        com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper<OrderSetDO> wrapper =
                Wrappers.<OrderSetDO>lambdaQuery()
                        .eq(OrderSetDO::getIsDeleted, false);

        if (StringUtils.hasText(query.getName())) {
            wrapper.like(OrderSetDO::getName, query.getName());
        }
        if (StringUtils.hasText(query.getTemplateCode())) {
            wrapper.like(OrderSetDO::getTemplateCode, query.getTemplateCode());
        }
        if (StringUtils.hasText(query.getBindingEnum())) {
            wrapper.eq(OrderSetDO::getBindingEnum, query.getBindingEnum());
        }
        if (StringUtils.hasText(query.getSceneEnum())) {
            wrapper.eq(OrderSetDO::getSceneEnum, query.getSceneEnum());
        }
        if (StringUtils.hasText(query.getStatusEnum())) {
            wrapper.eq(OrderSetDO::getStatusEnum, query.getStatusEnum());
        }

        wrapper.orderByDesc(OrderSetDO::getCreatedAt);

        com.baomidou.mybatisplus.core.metadata.IPage<OrderSetDO> result = orderSetMapper.selectPage(page, wrapper);

        List<OrderSetListVO> voList = result.getRecords().stream()
                .map(this::convertToListVO)
                .toList();

        PageDTO<OrderSetListVO> pageDTO = new PageDTO<>();
        pageDTO.setRows(voList);
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setPages(result.getPages());

        return pageDTO;
    }

    @Override
    public OrderSetDetailVO getOrderSetDetail(String id) {
        OrderSetDO orderSet = orderSetMapper.selectById(id);
        if (orderSet == null || Boolean.TRUE.equals(orderSet.getIsDeleted())) {
            throw new BusinessException("组套模板不存在");
        }

        List<OrderSetLineDO> lines = orderSetLineMapper.selectList(
                Wrappers.<OrderSetLineDO>lambdaQuery()
                        .eq(OrderSetLineDO::getOrderTemplateId, id)
                        .eq(OrderSetLineDO::getIsDeleted, false)
                        .orderByAsc(OrderSetLineDO::getSortOrder));

        return convertToDetailVO(orderSet, lines);
    }

    private OrderSetNameVO convertToNameVO(OrderSetDO orderSet) {
        OrderSetNameVO vo = new OrderSetNameVO();
        vo.setId(orderSet.getId());
        vo.setName(orderSet.getName());
        vo.setTemplateCode(orderSet.getTemplateCode());
        return vo;
    }

    private OrderSetListVO convertToListVO(OrderSetDO orderSet) {
        OrderSetListVO vo = new OrderSetListVO();
        vo.setId(orderSet.getId());
        vo.setTemplateCode(orderSet.getTemplateCode());
        vo.setName(orderSet.getName());
        vo.setDescription(orderSet.getDescription());
        vo.setBindingEnum(orderSet.getBindingEnum());
        vo.setSceneEnum(orderSet.getSceneEnum());
        vo.setStatusEnum(orderSet.getStatusEnum());
        vo.setTemplateKindEnum(orderSet.getTemplateKindEnum());
        vo.setSortWeight(orderSet.getSortWeight());
        vo.setRemark(orderSet.getRemark());
        vo.setCreatedAt(orderSet.getCreatedAt());
        vo.setUpdatedAt(orderSet.getUpdatedAt());
        return vo;
    }

    private OrderSetDetailVO convertToDetailVO(OrderSetDO orderSet, List<OrderSetLineDO> lines) {
        OrderSetDetailVO vo = new OrderSetDetailVO();
        vo.setId(orderSet.getId());
        vo.setObjectId(orderSet.getObjectId());
        vo.setBindingEnum(orderSet.getBindingEnum());
        vo.setSceneEnum(orderSet.getSceneEnum());
        vo.setTemplateCode(orderSet.getTemplateCode());
        vo.setName(orderSet.getName());
        vo.setDescription(orderSet.getDescription());
        vo.setSortWeight(orderSet.getSortWeight());
        vo.setStatusEnum(orderSet.getStatusEnum());
        vo.setRemark(orderSet.getRemark());
        vo.setTemplateKindEnum(orderSet.getTemplateKindEnum());
        vo.setDefaultChineseHerbsDoseQuantity(orderSet.getDefaultChineseHerbsDoseQuantity());
        vo.setDefaultSufferingFlag(orderSet.getDefaultSufferingFlag());
        vo.setTemplateSourceEnum(orderSet.getTemplateSourceEnum());
        vo.setCreatedAt(orderSet.getCreatedAt());
        vo.setUpdatedAt(orderSet.getUpdatedAt());
        vo.setCreatedBy(orderSet.getCreatedBy());
        vo.setUpdatedBy(orderSet.getUpdatedBy());

        List<OrderSetDetailVO.OrderSetLineVO> lineVOs = lines.stream()
                .map(this::convertToLineVO)
                .toList();
        vo.setLines(lineVOs);

        return vo;
    }

    private OrderSetDetailVO.OrderSetLineVO convertToLineVO(OrderSetLineDO line) {
        OrderSetDetailVO.OrderSetLineVO vo = new OrderSetDetailVO.OrderSetLineVO();
        vo.setId(line.getId());
        vo.setOrderTemplateId(line.getOrderTemplateId());
        vo.setLineKindEnum(line.getLineKindEnum());
        vo.setMedicationDefinitionId(line.getMedicationDefinitionId());
        vo.setActivityDefinitionId(line.getActivityDefinitionId());
        vo.setDeviceDefinitionId(line.getDeviceDefinitionId());
        vo.setGroupKey(line.getGroupKey());
        vo.setSortOrder(line.getSortOrder());
        vo.setDefaultQuantity(line.getDefaultQuantity());
        vo.setDefaultUnitCode(line.getDefaultUnitCode());
        vo.setDefaultDays(line.getDefaultDays());
        vo.setFrequencyId(line.getFrequencyId());
        vo.setRouteCode(line.getRouteCode());
        vo.setDosageText(line.getDosageText());
        vo.setDefaultPerformOrgId(line.getDefaultPerformOrgId());
        vo.setLineRemark(line.getLineRemark());
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public OrderSetOperationVO addOrderSet(OrderSetAddDTO dto) {
        UserDTO user = currentUser();
        validateHeader(dto.getBindingEnum(), dto.getSceneEnum(), dto.getTemplateKindEnum());
        validateLines(dto.getLines());

        OrderSetDO orderSet = OrderSetConverter.fromAddDTO(dto, user.getTenantId(), user.getId());
        normalizeTcmFields(orderSet);
        if (orderSetMapper.insert(orderSet) != 1) {
            throw new BusinessException("新增组套模板失败");
        }

        List<OrderSetLineDO> lines = dto.getLines().stream()
                .map(line -> OrderSetConverter.newLine(line, orderSet.getId(), user.getTenantId(), user.getId()))
                .toList();
        if (!lines.isEmpty() && orderSetLineMapper.insertBatch(lines) != lines.size()) {
            throw new BusinessException("新增组套模板明细失败");
        }

        if (lines.isEmpty()) {
            return OrderSetConverter.toOperationVO(orderSet, "组套已保存为草稿；空壳组套不可发布");
        }
        int affected = orderSetMapper.updateStatusByVersion(orderSet.getId(), user.getTenantId(),
                OrderSetStatusEnum.ACTIVE.name(), user.getId(), orderSet.getVersion());
        ensureOneRow(affected);
        orderSet.setStatusEnum(OrderSetStatusEnum.ACTIVE.name());
        orderSet.setVersion(orderSet.getVersion() + 1);
        return OrderSetConverter.toOperationVO(orderSet, "组套保存并发布成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public OrderSetOperationVO modifyOrderSet(OrderSetModifyDTO dto) {
        UserDTO user = currentUser();
        OrderSetDO orderSet = findOrderSet(dto.getId(), user.getTenantId());
        checkVersion(dto.getVersion(), orderSet.getVersion());
        if (OrderSetStatusEnum.RETIRED.name().equals(orderSet.getStatusEnum())) {
            throw new BusinessException("已停用的组套不可修改");
        }
        if (!orderSet.getObjectId().equals(dto.getObjectId())
                || !orderSet.getBindingEnum().equals(dto.getBindingEnum())
                || !orderSet.getTemplateKindEnum().equals(dto.getTemplateKindEnum())) {
            throw new BusinessException("修改接口不允许改变组套归属、绑定范围或组套种类，请使用另存/复制流程");
        }
        validateHeader(dto.getBindingEnum(), dto.getSceneEnum(), dto.getTemplateKindEnum());
        validateLines(dto.getLines());
        if (OrderSetStatusEnum.ACTIVE.name().equals(orderSet.getStatusEnum()) && dto.getLines().isEmpty()) {
            throw new BusinessException("已发布组套至少需要一条有效明细");
        }

        List<OrderSetLineDO> existingLines = orderSetLineMapper.selectList(
                Wrappers.<OrderSetLineDO>lambdaQuery()
                        .eq(OrderSetLineDO::getTenantId, user.getTenantId())
                        .eq(OrderSetLineDO::getOrderTemplateId, orderSet.getId()));
        Map<String, OrderSetLineDO> existingById = existingLines.stream()
                .collect(Collectors.toMap(OrderSetLineDO::getId, Function.identity()));

        Set<String> submittedIds = new HashSet<>();
        List<OrderSetLineDO> newLines = new ArrayList<>();
        List<OrderSetLineDO> changedLines = new ArrayList<>();
        Map<String, Integer> oldLineVersions = new LinkedHashMap<>();
        for (OrderSetLineDTO lineDTO : dto.getLines()) {
            if (!StringUtils.hasText(lineDTO.getId())) {
                newLines.add(OrderSetConverter.newLine(lineDTO, orderSet.getId(),
                        user.getTenantId(), user.getId()));
                continue;
            }
            if (!submittedIds.add(lineDTO.getId())) {
                throw new BusinessException("组套明细编号重复：" + lineDTO.getId());
            }
            OrderSetLineDO line = existingById.get(lineDTO.getId());
            if (line == null) {
                throw new BusinessException("组套明细不存在或不属于当前组套：" + lineDTO.getId());
            }
            oldLineVersions.put(line.getId(), line.getVersion());
            OrderSetConverter.applyLineDTO(lineDTO, line, user.getId());
            changedLines.add(line);
        }
        List<String> removedIds = existingLines.stream()
                .map(OrderSetLineDO::getId)
                .filter(id -> !submittedIds.contains(id))
                .toList();

        OrderSetConverter.applyModifyDTO(dto, orderSet, user.getId());
        normalizeTcmFields(orderSet);
        ensureOneRow(orderSetMapper.updateContentByVersion(orderSet, dto.getVersion()));
        orderSet.setVersion(orderSet.getVersion() + 1);

        if (!newLines.isEmpty() && orderSetLineMapper.insertBatch(newLines) != newLines.size()) {
            throw new BusinessException("新增组套模板明细失败");
        }
        for (OrderSetLineDO line : changedLines) {
            ensureOneRow(orderSetLineMapper.updateContentByVersion(line, oldLineVersions.get(line.getId())));
        }
        if (!removedIds.isEmpty()) {
            int affected = orderSetLineMapper.softDeleteByIds(removedIds, user.getTenantId(),
                    orderSet.getId(), user.getId());
            if (affected != removedIds.size()) {
                throw new BusinessException(CONCURRENT_MESSAGE);
            }
        }
        return OrderSetConverter.toOperationVO(orderSet, "组套修改成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String removeOrderSet(String id) {
        UserDTO user = currentUser();
        OrderSetDO orderSet = findOrderSet(id, user.getTenantId());
        ensureOneRow(orderSetMapper.softDeleteByVersion(id, user.getTenantId(),
                user.getId(), orderSet.getVersion()));
        orderSetLineMapper.softDeleteByTemplateId(user.getTenantId(), id, user.getId());
        return id;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public OrderSetOperationVO modifyOrderSetStatus(OrderSetStatusDTO dto) {
        UserDTO user = currentUser();
        OrderSetDO orderSet = findOrderSet(dto.getId(), user.getTenantId());
        checkVersion(dto.getVersion(), orderSet.getVersion());
        OrderSetActionEnum action = OrderSetActionEnum.from(dto.getOperationType());

        if (action == OrderSetActionEnum.PUBLISH) {
            return publish(orderSet, user);
        }
        return retire(orderSet, user);
    }

    private OrderSetOperationVO publish(OrderSetDO orderSet, UserDTO user) {
        if (OrderSetStatusEnum.ACTIVE.name().equals(orderSet.getStatusEnum())) {
            return OrderSetConverter.toOperationVO(orderSet, "组套已经处于启用状态");
        }
        if (OrderSetStatusEnum.RETIRED.name().equals(orderSet.getStatusEnum())) {
            throw new BusinessException("已停用的组套不可重新发布");
        }
        Long lineCount = orderSetLineMapper.selectCount(Wrappers.<OrderSetLineDO>lambdaQuery()
                .eq(OrderSetLineDO::getTenantId, user.getTenantId())
                .eq(OrderSetLineDO::getOrderTemplateId, orderSet.getId()));
        if (lineCount == 0) {
            throw new BusinessException("空壳组套不可发布");
        }
        ensureOneRow(orderSetMapper.updateStatusByVersion(orderSet.getId(), user.getTenantId(),
                OrderSetStatusEnum.ACTIVE.name(), user.getId(), orderSet.getVersion()));
        orderSet.setStatusEnum(OrderSetStatusEnum.ACTIVE.name());
        orderSet.setVersion(orderSet.getVersion() + 1);
        return OrderSetConverter.toOperationVO(orderSet, "组套发布成功");
    }

    private OrderSetOperationVO retire(OrderSetDO orderSet, UserDTO user) {
        if (OrderSetStatusEnum.RETIRED.name().equals(orderSet.getStatusEnum())) {
            return OrderSetConverter.toOperationVO(orderSet, "组套已经处于停用状态");
        }
        if (OrderSetStatusEnum.DRAFT.name().equals(orderSet.getStatusEnum())) {
            throw new BusinessException("草稿组套无需停用");
        }
        ensureOneRow(orderSetMapper.updateStatusByVersion(orderSet.getId(), user.getTenantId(),
                OrderSetStatusEnum.RETIRED.name(), user.getId(), orderSet.getVersion()));
        orderSet.setStatusEnum(OrderSetStatusEnum.RETIRED.name());
        orderSet.setVersion(orderSet.getVersion() + 1);
        return OrderSetConverter.toOperationVO(orderSet, "组套停用成功");
    }

    private OrderSetDO findOrderSet(String id, String tenantId) {
        OrderSetDO entity = orderSetMapper.selectOne(Wrappers.<OrderSetDO>lambdaQuery()
                .eq(OrderSetDO::getId, id)
                .eq(OrderSetDO::getTenantId, tenantId));
        if (entity == null) {
            throw new BusinessException("组套模板不存在或无权访问");
        }
        return entity;
    }

    private UserDTO currentUser() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            if (user == null || !StringUtils.hasText(user.getId()) || !StringUtils.hasText(user.getTenantId())) {
                throw new BusinessException("无法获取当前用户或租户信息");
            }
            return user;
        } catch (BusinessException e) {
            throw e;
        } catch (Exception e) {
            throw new BusinessException("解析当前用户信息失败：" + e.getMessage());
        }
    }

    private void validateHeader(String binding, String scene, String kind) {
        parseEnum(OrderSetBindingEnum.class, binding, "绑定类型");
        parseEnum(OrderSetSceneEnum.class, scene, "适用场景");
        parseEnum(OrderSetKindEnum.class, kind, "组套种类");
    }

    private void validateLines(List<OrderSetLineDTO> lines) {
        for (OrderSetLineDTO line : lines) {
            OrderSetLineKindEnum kind = parseEnum(OrderSetLineKindEnum.class,
                    line.getLineKindEnum(), "明细类型");
            int referenceCount = (StringUtils.hasText(line.getMedicationDefinitionId()) ? 1 : 0)
                    + (StringUtils.hasText(line.getActivityDefinitionId()) ? 1 : 0)
                    + (StringUtils.hasText(line.getDeviceDefinitionId()) ? 1 : 0);
            boolean expectedReferencePresent = switch (kind) {
                case MEDICATION -> StringUtils.hasText(line.getMedicationDefinitionId());
                case SERVICE -> StringUtils.hasText(line.getActivityDefinitionId());
                case DEVICE -> StringUtils.hasText(line.getDeviceDefinitionId());
            };
            if (referenceCount != 1 || !expectedReferencePresent) {
                throw new BusinessException(kind.name() + "明细必须且只能填写对应的一种定义编号");
            }
        }
    }

    private void normalizeTcmFields(OrderSetDO entity) {
        if (!OrderSetKindEnum.TCM.name().equals(entity.getTemplateKindEnum())) {
            entity.setDefaultChineseHerbsDoseQuantity(null);
            entity.setDefaultSufferingFlag(null);
        }
    }

    private <E extends Enum<E>> E parseEnum(Class<E> type, String value, String label) {
        try {
            return Enum.valueOf(type, value);
        } catch (IllegalArgumentException | NullPointerException e) {
            throw new BusinessException(label + "取值不合法：" + value);
        }
    }

    private void checkVersion(Integer requestVersion, Integer databaseVersion) {
        if (!requestVersion.equals(databaseVersion)) {
            throw new BusinessException(CONCURRENT_MESSAGE);
        }
    }

    private void ensureOneRow(int affected) {
        if (affected != 1) {
            throw new BusinessException(CONCURRENT_MESSAGE);
        }
    }
}
