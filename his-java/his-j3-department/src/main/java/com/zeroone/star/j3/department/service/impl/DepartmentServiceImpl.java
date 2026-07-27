package com.zeroone.star.j3.department.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
import com.zeroone.star.j3.department.entity.SequenceDO;
import com.zeroone.star.j3.department.mapper.DepartmentMapper;
import com.zeroone.star.j3.department.service.DepartmentService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentUpdateDTO;
import com.zeroone.star.project.enums.j3.department.ClassEnum;
import com.zeroone.star.project.enums.j3.department.StatusEnum;
import com.zeroone.star.project.enums.j3.department.TypeEnum;
import com.zeroone.star.project.exception.j3.department.DepartmentActiveOrInactiveStatusException;
import com.zeroone.star.project.exception.j3.department.DepartmentNotFoundException;
import com.zeroone.star.project.exception.j3.department.DoctorNotFoundException;
import com.zeroone.star.project.exception.j3.department.EmptyParentDepartmentException;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import com.zeroone.star.project.vo.j3.department.DepartmentVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：科室管理服务层实现类
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Service
@Slf4j
public class DepartmentServiceImpl implements DepartmentService {
    @Resource
    private DepartmentMapper departmentMapper;
    @Resource
    private UserHolder userHolder;

    @Override
    public PageDTO<DepartmentVO> getDepartmentPage(DepartmentQuery query) {
        Page<DepartmentDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<DepartmentDO> queryWrapper = new LambdaQueryWrapper<>();

        if (StringUtils.hasText(query.getKeyword())) {
            String keyword = query.getKeyword().trim();
            queryWrapper.and(wrapper -> wrapper
                    .like(DepartmentDO::getName, keyword)
                    .or()
                    .like(DepartmentDO::getYbNo, keyword)
                    .or()
                    .like(DepartmentDO::getYbName, keyword)
                    .or()
                    .like(DepartmentDO::getMedinsId, keyword)
                    .or()
                    .like(DepartmentDO::getMedinsAdmdvs, keyword)
                    .or()
                    .like(DepartmentDO::getPyStr, keyword)
                    .or()
                    .like(DepartmentDO::getWbStr, keyword));
        }
        if (StringUtils.hasText(query.getTypeEnum())) {
            queryWrapper.eq(DepartmentDO::getTypeEnum, query.getTypeEnum().trim());
        }
        if (StringUtils.hasText(query.getClassEnum())) {
            queryWrapper.eq(DepartmentDO::getClassEnum, query.getClassEnum().trim());
        }
        if (StringUtils.hasText(query.getStatusEnum())) {
            queryWrapper.eq(DepartmentDO::getStatusEnum, query.getStatusEnum().trim());
        }

        queryWrapper.orderByAsc(DepartmentDO::getBusNo)
                .orderByAsc(DepartmentDO::getDisplayOrder)
                .orderByAsc(DepartmentDO::getId);

        Page<DepartmentDO> result = departmentMapper.selectPage(page, queryWrapper);
        return PageDTO.create(result, this::toDepartmentVO);
    }

    @Override
    public List<DepartmentTreeVO> getDepartmentTree(DepartmentTreeQuery query) {
        LambdaQueryWrapper<DepartmentDO> parentWrapper = new LambdaQueryWrapper<>();
        parentWrapper.like(query.getKeyword() != null, DepartmentDO::getName, query.getKeyword())
                .eq(query.getTypeEnum() != null, DepartmentDO::getTypeEnum, query.getTypeEnum())
                .eq(query.getClassEnum() != null, DepartmentDO::getClassEnum, query.getClassEnum())
                .eq(query.getStatusEnum() != null, DepartmentDO::getStatusEnum, query.getStatusEnum())
                .isNull(DepartmentDO::getParentId)
                .orderByAsc(DepartmentDO::getCreatedAt);
        List<DepartmentDO> parentDepartmentList = departmentMapper.selectList(parentWrapper);

        LambdaQueryWrapper<DepartmentDO> childWrapper = new LambdaQueryWrapper<>();
        childWrapper.like(query.getKeyword() != null, DepartmentDO::getName, query.getKeyword())
                .eq(query.getTypeEnum() != null, DepartmentDO::getTypeEnum, query.getTypeEnum())
                .eq(query.getClassEnum() != null, DepartmentDO::getClassEnum, query.getClassEnum())
                .eq(query.getStatusEnum() != null, DepartmentDO::getStatusEnum, query.getStatusEnum())
                .isNotNull(DepartmentDO::getParentId)
                .orderByAsc(DepartmentDO::getCreatedAt);
        List<DepartmentDO> childrenDepartmentList = departmentMapper.selectList(childWrapper);

        Map<Long, DepartmentTreeVO> map = new LinkedHashMap<>();
        for (DepartmentDO departmentDO : parentDepartmentList) {
            map.put(departmentDO.getId(), departmentDO.toTreeVO());
        }
        for (DepartmentDO departmentDO : childrenDepartmentList) {
            map.put(departmentDO.getId(), departmentDO.toTreeVO());
        }

        if (!childrenDepartmentList.isEmpty()) {
            Set<Long> missingParentIds = new LinkedHashSet<>();
            for (DepartmentDO departmentDO : childrenDepartmentList) {
                if (!map.containsKey(departmentDO.getParentId())) {
                    missingParentIds.add(departmentDO.getParentId());
                }
            }

            while (!missingParentIds.isEmpty()) {
                List<DepartmentDO> missingDepartmentList = departmentMapper.selectByIds(missingParentIds);
                Set<Long> nextMissing = new LinkedHashSet<>();
                for (DepartmentDO missingDepartment : missingDepartmentList) {
                    if (!map.containsKey(missingDepartment.getId())) {
                        map.put(missingDepartment.getId(), missingDepartment.toTreeVO());
                    }
                    if (missingDepartment.getParentId() != null && !map.containsKey(missingDepartment.getParentId())) {
                        nextMissing.add(missingDepartment.getParentId());
                    }
                }
                missingParentIds = nextMissing;
            }
        }

        for (DepartmentDO departmentDO : childrenDepartmentList) {
            DepartmentTreeVO current = map.get(departmentDO.getId());
            DepartmentTreeVO parent = map.get(departmentDO.getParentId());
            if (parent != null) {
                if (parent.getChildren() == null) {
                    parent.setChildren(new ArrayList<>());
                }
                parent.getChildren().add(current);
            }
        }

        Set<Long> matchedChildIds = childrenDepartmentList.stream()
                .map(DepartmentDO::getId)
                .collect(Collectors.toSet());

        for (Map.Entry<Long, DepartmentTreeVO> entry : map.entrySet()) {
            DepartmentTreeVO current = entry.getValue();
            if (current.getParentId() == null || matchedChildIds.contains(entry.getKey())) {
                continue;
            }
            DepartmentTreeVO parent = map.get(current.getParentId());
            if (parent != null) {
                if (parent.getChildren() == null) {
                    parent.setChildren(new ArrayList<>());
                }
                if (!parent.getChildren().contains(current)) {
                    parent.getChildren().add(current);
                }
            }
        }

        List<DepartmentTreeVO> result = new ArrayList<>();
        for (DepartmentDO departmentDO : parentDepartmentList) {
            result.add(map.get(departmentDO.getId()));
        }

        return result;
    }

    @Override
    public DepartmentDetailVO getDepartmentDetail(Long id) {
        DepartmentDO departmentDO = departmentMapper.selectById(id);
        if (departmentDO == null) {
            throw new DepartmentNotFoundException("该科室不存在，ID: " + id);
        }
        return departmentDO.toDetailVO();
    }

    @Override
    @Transactional
    public void createDepartment(DepartmentCreateDTO dto) {
        Long parentId = dto.getParentId();
        DepartmentDO parentDepartment = null;
        if (Objects.equals(TypeEnum.getDescByValue(dto.getTypeEnum()), "DEPARTMENT")) {
            if (parentId == null) {
                throw new EmptyParentDepartmentException("科室的上级科室不能为空");
            } else {
                parentDepartment = departmentMapper.selectById(parentId);
                if (parentDepartment == null) {
                    throw new DepartmentNotFoundException("该科室的上级科室不存在，ID: " + parentId);
                }
            }
        } else {
            if (parentId != null) {
                throw new EmptyParentDepartmentException("医院的上级科室必须为空");
            }
        }
        Long defDoctorId = dto.getDefDoctorId();
        if (defDoctorId != null) {
            if (departmentMapper.isDoctorExist(defDoctorId) == 0) {
                throw new DoctorNotFoundException("该医务人员不存在，ID: " + defDoctorId);
            }
        }
//        TODO：用户功能暂未实现，测试无法通过，先用固定值代替
//        UserDTO currentUser = userHolder.getCurrentUser();
//        Long currentUserId = Long.valueOf(currentUser.getId());
        Long currentUserId = 1L;
        String seqKey = "1:OR";
        Long currentValue = departmentMapper.getCurrentValueFromSequence(seqKey);
        if (currentValue == null) {
            currentValue = 0L;
            SequenceDO sequenceDO = SequenceDO.builder()
                    .seqKey(seqKey)
                    .currentValue(currentValue)
                    .updatedAt(LocalDateTime.now())
                    .build();
            departmentMapper.addSeqKeyFromSequence(sequenceDO);
        }
        Long newValue = currentValue + 1;
        String busNo;
        String parentBusNO = "";
        String typeEnum = TypeEnum.getDescByValue(dto.getTypeEnum());
        if (typeEnum.equals(TypeEnum.HOSPITAL.getDesc())) {
            busNo = String.format("OR%04d", newValue);
        } else {
            if (parentDepartment != null) {
                parentBusNO = parentDepartment.getBusNo();
            }
            busNo = parentBusNO + "." + String.format("OR%04d", newValue);
        }
        departmentMapper.updateCurrentValue(seqKey, newValue);
        Long tenantId = departmentMapper.getTenantIdByUserId(currentUserId);
        DepartmentDO departmentDO = DepartmentDO.builder()
                .tenantId(tenantId)
                .createdAt(LocalDateTime.now())
                .updatedAt(LocalDateTime.now())
                .createdBy(currentUserId)
                .updatedBy(currentUserId)
                .name(dto.getName())
                .busNo(busNo)
                .parentId(parentId)
                .typeEnum(TypeEnum.getDescByValue(dto.getTypeEnum()))
                .classEnum(ClassEnum.getDescByValue(dto.getClassEnum()))
                .statusEnum(StatusEnum.ACTIVE.getDesc())
                .activeFlag(true)
                .displayOrder(dto.getDisplayOrder())
                .defDoctorId(Long.valueOf(defDoctorId))
                .pyStr(dto.getPyStr())
                .wbStr(dto.getWbStr())
                .ybNo(dto.getYbNo())
                .ybName(dto.getYbName())
                .medinsId(dto.getMedinsId())
                .medinsAdmdvs(dto.getMedinsAdmdvs())
                .medinsType(dto.getMedinsType())
                .medinsLv(dto.getMedinsLv())
                .caty(dto.getCaty())
                .build();
        departmentMapper.insert(departmentDO);

        Long parentUnitId = null;
        if (parentBusNO != null && !parentBusNO.isEmpty()) {
            parentUnitId = departmentMapper.selectParentUnitIdByBusNo(parentBusNO);
        }
        DepartmentUnitDO departmentUnitDO = DepartmentUnitDO.builder()
                .id(IdWorker.getId())
                .code(busNo)
                .displayName(dto.getName())
                .parentId(parentUnitId)
                .sortOrder(dto.getDisplayOrder())
                .tenantId(tenantId)
                .createdAt(LocalDateTime.now())
                .updatedAt(LocalDateTime.now())
                .createdBy(currentUserId)
                .updatedBy(currentUserId)
                .build();
        departmentMapper.insertUnit(departmentUnitDO);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void updateDepartment(String id, DepartmentUpdateDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("修改内容不能为空");
        }

        Long departmentId = parseRequiredId(id, "科室ID");
        DepartmentDO existing = departmentMapper.selectUpdateDepartmentById(toSqlId(departmentId));
        if (existing == null) {
            throw new IllegalArgumentException("科室不存在，请检查ID是否正确");
        }

        Long currentUserId = resolveCurrentUserId(null);
        if (currentUserId != null) {
            Long currentTenantId = departmentMapper.getTenantIdByUserId(currentUserId);
            if (currentTenantId != null && !Objects.equals(currentTenantId, existing.getTenantId())) {
                throw new IllegalArgumentException("无权修改其他租户的科室");
            }
        }

        String name = trimToNull(dto.getName());
        if (name == null) {
            throw new IllegalArgumentException("科室名称不能为空");
        }
        String typeEnum = normalizeTypeEnum(dto.getTypeEnum());
        String classEnum = dto.getClassEnum() == null
                ? existing.getClassEnum()
                : normalizeClassEnum(dto.getClassEnum());

        boolean parentIdChanged = Boolean.TRUE.equals(dto.getParentIdChanged());
        Long parentId = parentIdChanged
                ? parseOptionalId(dto.getParentId(), "上级科室ID")
                : existing.getParentId();

        if (TypeEnum.HOSPITAL.getDesc().equals(typeEnum) && parentId != null) {
            throw new IllegalArgumentException("医院的上级组织必须为空");
        }
        if (TypeEnum.DEPARTMENT.getDesc().equals(typeEnum) && parentId == null) {
            throw new IllegalArgumentException("科室的上级组织不能为空");
        }

        DepartmentDO parent = parentIdChanged
                ? validateParentChain(parentId, existing)
                : null;
        if (isNameDuplicate(name, parentId, existing)) {
            throw new IllegalArgumentException("同一上级组织下已存在名称为「" + name + "」的科室");
        }

        boolean defDoctorChanged = dto.getDefDoctorId() != null;
        Long defDoctorId = defDoctorChanged
                ? parseOptionalId(dto.getDefDoctorId(), "默认医师ID")
                : existing.getDefDoctorId();
        if (defDoctorChanged && defDoctorId != null
                && departmentMapper.isDoctorAvailable(
                toSqlId(defDoctorId), toSqlId(existing.getTenantId())) == 0) {
            throw new IllegalArgumentException("默认医师不存在、已停用或不属于当前租户，ID: " + defDoctorId);
        }

        LocalDateTime updatedAt = LocalDateTime.now();
        Long updatedBy = currentUserId == null ? existing.getUpdatedBy() : currentUserId;
        Integer displayOrder = dto.getDisplayOrder() == null
                ? existing.getDisplayOrder()
                : dto.getDisplayOrder();
        LambdaUpdateWrapper<DepartmentDO> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.eq(DepartmentDO::getId, toSqlId(departmentId))
                .set(DepartmentDO::getName, name)
                .set(DepartmentDO::getTypeEnum, typeEnum)
                .set(dto.getClassEnum() != null, DepartmentDO::getClassEnum, classEnum)
                .set(dto.getYbNo() != null, DepartmentDO::getYbNo, trimToNull(dto.getYbNo()))
                .set(dto.getYbName() != null, DepartmentDO::getYbName, trimToNull(dto.getYbName()))
                .set(dto.getMedinsId() != null, DepartmentDO::getMedinsId, trimToNull(dto.getMedinsId()))
                .set(dto.getMedinsAdmdvs() != null, DepartmentDO::getMedinsAdmdvs, trimToNull(dto.getMedinsAdmdvs()))
                .set(dto.getMedinsType() != null, DepartmentDO::getMedinsType, trimToNull(dto.getMedinsType()))
                .set(dto.getMedinsLv() != null, DepartmentDO::getMedinsLv, trimToNull(dto.getMedinsLv()))
                .set(dto.getPyStr() != null, DepartmentDO::getPyStr, trimToNull(dto.getPyStr()))
                .set(dto.getWbStr() != null, DepartmentDO::getWbStr, trimToNull(dto.getWbStr()))
                .set(defDoctorChanged, DepartmentDO::getDefDoctorId, toSqlId(defDoctorId))
                .set(dto.getCaty() != null, DepartmentDO::getCaty, trimToNull(dto.getCaty()))
                .set(dto.getDisplayOrder() != null, DepartmentDO::getDisplayOrder, displayOrder)
                .set(DepartmentDO::getUpdatedAt, updatedAt)
                .set(DepartmentDO::getUpdatedBy, updatedBy);

        if (parentIdChanged) {
            updateWrapper.set(DepartmentDO::getParentId, toSqlId(parentId));
        }
        if (existing.getVersion() != null) {
            updateWrapper.eq(DepartmentDO::getVersion, existing.getVersion())
                    .set(DepartmentDO::getVersion, existing.getVersion() + 1);
        }

        if (departmentMapper.update(updateWrapper) != 1) {
            throw new IllegalStateException("科室信息已被其他用户修改，请刷新后重试");
        }

        // busNo/code 作为稳定业务标识，调整上级时只同步两张表的 parent_id。
        syncOrganizationUnit(existing, parent, name, displayOrder,
                parentIdChanged, updatedAt, updatedBy);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void deleteDepartment(String id) {
        Long parsedDepartmentId = parseRequiredId(id, "科室ID");
        String departmentId = toSqlId(parsedDepartmentId);
        DepartmentDO existing = departmentMapper.selectUpdateDepartmentById(departmentId);
        if (existing == null) {
            throw new IllegalArgumentException("科室不存在或已删除");
        }

        Long currentUserId = resolveCurrentUserId(null);
        if (currentUserId != null) {
            Long currentTenantId = departmentMapper.getTenantIdByUserId(currentUserId);
            if (currentTenantId != null && !Objects.equals(currentTenantId, existing.getTenantId())) {
                throw new IllegalArgumentException("无权删除其他租户的科室");
            }
        }

        String tenantId = toSqlId(existing.getTenantId());
        if (departmentMapper.countUndeletedChildDepartments(departmentId, tenantId) > 0) {
            throw new IllegalStateException("该科室存在下级科室，请先处理关联数据后再删除");
        }
        if (departmentMapper.existsDeleteBusinessReference(departmentId, tenantId) > 0) {
            throw new IllegalStateException("该科室存在业务引用，请先处理关联数据后再删除");
        }
        if (existing.getVersion() == null) {
            throw new IllegalStateException("科室版本信息缺失，无法安全删除");
        }

        LocalDateTime deletedAt = LocalDateTime.now();
        Long auditUserId = currentUserId != null
                ? currentUserId
                : (existing.getUpdatedBy() != null ? existing.getUpdatedBy() : existing.getCreatedBy());
        String deletedBy = toSqlId(auditUserId);

        int deleted = departmentMapper.softDeleteDepartment(
                departmentId,
                tenantId,
                existing.getVersion(),
                deletedAt,
                deletedBy
        );
        if (deleted != 1) {
            throw new IllegalStateException("科室信息已被其他用户修改，请刷新后重试");
        }

        Long unitId = departmentMapper.selectUnitIdByBusNo(existing.getBusNo(), tenantId);
        if (unitId != null) {
            int unitDeleted = departmentMapper.softDeleteUnitByCode(
                    existing.getBusNo(),
                    tenantId,
                    deletedAt,
                    deletedBy
            );
            if (unitDeleted != 1) {
                throw new IllegalStateException("关联组织单元删除失败");
            }
        }
    }

    @Override
    public void activeOrInactiveDepartment(DepartmentBatchDTO dto) {
        Integer isEnabled = dto.getIsEnabled();
        List<Long> ids = dto.getIds();

        LambdaQueryWrapper<DepartmentDO> selectWrapper = new LambdaQueryWrapper<>();
        selectWrapper.select(
                DepartmentDO::getId,
                DepartmentDO::getParentId,
                DepartmentDO::getActiveFlag);
        List<DepartmentDO> departmentDOList = departmentMapper.selectList(selectWrapper);

        Map<Long, DepartmentDO> map = new HashMap<>();
        Map<Long, List<Long>> parentChildMap = new HashMap<>();
        for (DepartmentDO departmentDO : departmentDOList) {
            map.put(departmentDO.getId(), departmentDO);
            if (departmentDO.getParentId() != null) {
                Long parentId = departmentDO.getParentId();
                List<Long> childrenIds = parentChildMap.computeIfAbsent(parentId, k -> new ArrayList<>());
                childrenIds.add(departmentDO.getId());
            }
        }

        if (isEnabled == 1) {
            for (Long id : ids) {
                Long current = id;
                while (current != null) {
                    DepartmentDO parent = map.get(current);
                    if (!current.equals(id) && !parent.getActiveFlag()) {
                        throw new DepartmentActiveOrInactiveStatusException("无法启用部门，其上级部门处于禁用状态");
                    }
                    current = parent.getParentId();
                }
            }
        } else {
            Set<Long> idsSet = new HashSet<>(ids);
            for (Long id : ids) {
                Queue<Long> queue = new LinkedList<>();
                queue.add(id);
                while (!queue.isEmpty()) {
                    Long currentId = queue.poll();
                    List<Long> childrenIds = parentChildMap.get(currentId);
                    if (childrenIds != null) {
                        for (Long childrenId : childrenIds) {
                            DepartmentDO childDepartmentDO = map.get(childrenId);
                            if (childDepartmentDO != null && childDepartmentDO.getActiveFlag()) {
                                if (!idsSet.contains(childrenId)) {
                                    throw new DepartmentActiveOrInactiveStatusException("无法禁用部门，其下级部门处于启用状态");
                                }
                            }
                            queue.add(childrenId);
                        }
                    }
                }
            }
        }

        LambdaUpdateWrapper<DepartmentDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.in(DepartmentDO::getId, ids)
                .set(DepartmentDO::getStatusEnum, StatusEnum.getDescByValue(isEnabled))
                .set(DepartmentDO::getActiveFlag, isEnabled == 1);
        departmentMapper.update(wrapper);
    }

    private DepartmentVO toDepartmentVO(DepartmentDO departmentDO) {
        DepartmentVO departmentVO = new DepartmentVO();
        departmentVO.setId(departmentDO.getId() == null ? null : departmentDO.getId().toString());
        departmentVO.setName(departmentDO.getName());
        departmentVO.setBusNo(departmentDO.getBusNo());
        departmentVO.setParentId(departmentDO.getParentId() == null ? null : departmentDO.getParentId().toString());
        departmentVO.setTypeEnum(departmentDO.getTypeEnum());
        departmentVO.setTypeEnumDictText(TypeEnum.getDictTextByDesc(departmentDO.getTypeEnum()));
        departmentVO.setClassEnum(departmentDO.getClassEnum());
        departmentVO.setClassEnumDictText(ClassEnum.getDictTextByDesc(departmentDO.getClassEnum()));
        departmentVO.setStatusEnum(departmentDO.getStatusEnum());
        departmentVO.setStatusEnumDictText(StatusEnum.getDictTextByDesc(departmentDO.getStatusEnum()));
        departmentVO.setActiveFlag(departmentDO.getActiveFlag() == null
                ? null
                : (departmentDO.getActiveFlag() ? 1 : 0));
        departmentVO.setYbNo(departmentDO.getYbNo());
        departmentVO.setYbName(departmentDO.getYbName());
        departmentVO.setMedinsId(departmentDO.getMedinsId());
        departmentVO.setMedinsAdmdvs(departmentDO.getMedinsAdmdvs());
        departmentVO.setMedinsType(departmentDO.getMedinsType());
        departmentVO.setMedinsLv(departmentDO.getMedinsLv());
        departmentVO.setPyStr(departmentDO.getPyStr());
        departmentVO.setWbStr(departmentDO.getWbStr());
        departmentVO.setDefDoctorId(departmentDO.getDefDoctorId() == null
                ? null
                : departmentDO.getDefDoctorId().toString());
        departmentVO.setCaty(departmentDO.getCaty());
        departmentVO.setDisplayOrder(departmentDO.getDisplayOrder());
        return departmentVO;
    }

    private Long parseRequiredId(String value, String fieldName) {
        Long id = parseOptionalId(value, fieldName);
        if (id == null) {
            throw new IllegalArgumentException(fieldName + "不能为空");
        }
        return id;
    }

    private Long parseOptionalId(String value, String fieldName) {
        if (!StringUtils.hasText(value)) {
            return null;
        }
        try {
            long id = Long.parseLong(value.trim());
            if (id <= 0) {
                throw new NumberFormatException();
            }
            return id;
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException(fieldName + "格式不正确");
        }
    }

    private String normalizeTypeEnum(String value) {
        String typeEnum = trimToNull(value);
        if (typeEnum == null) {
            throw new IllegalArgumentException("科室类型不能为空");
        }
        typeEnum = typeEnum.toUpperCase(Locale.ROOT);
        if (TypeEnum.getValueByDesc(typeEnum) == null) {
            throw new IllegalArgumentException("科室类型仅支持 HOSPITAL 或 DEPARTMENT");
        }
        return typeEnum;
    }

    private String normalizeClassEnum(String value) {
        String classEnum = trimToNull(value);
        if (classEnum == null) {
            return null;
        }
        classEnum = classEnum.toUpperCase(Locale.ROOT);
        if (ClassEnum.getValueByDesc(classEnum) == null) {
            throw new IllegalArgumentException("科室类别不合法");
        }
        return classEnum;
    }

    private String trimToNull(String value) {
        return StringUtils.hasText(value) ? value.trim() : null;
    }

    private DepartmentDO validateParentChain(Long parentId, DepartmentDO currentDepartment) {
        if (parentId == null) {
            return null;
        }

        DepartmentDO directParent = null;
        Set<Long> visited = new HashSet<>();
        Long currentId = parentId;
        while (currentId != null) {
            if (Objects.equals(currentId, currentDepartment.getId())) {
                throw new IllegalArgumentException("不能将科室移动到自身或其子科室下");
            }
            if (!visited.add(currentId)) {
                throw new IllegalArgumentException("上级科室链存在循环引用");
            }

            DepartmentDO ancestor = departmentMapper.selectUpdateDepartmentById(toSqlId(currentId));
            if (ancestor == null) {
                throw new IllegalArgumentException("指定的上级科室不存在，ID: " + currentId);
            }
            if (!Objects.equals(ancestor.getTenantId(), currentDepartment.getTenantId())) {
                throw new IllegalArgumentException("不能选择其他租户的科室作为上级");
            }
            if (Boolean.TRUE.equals(currentDepartment.getActiveFlag())
                    && !Boolean.TRUE.equals(ancestor.getActiveFlag())) {
                throw new IllegalArgumentException("启用中的科室不能挂在已停用的上级科室下");
            }
            if (directParent == null) {
                directParent = ancestor;
            }
            currentId = ancestor.getParentId();
        }
        return directParent;
    }

    private boolean isNameDuplicate(String name, Long parentId, DepartmentDO existing) {
        LambdaQueryWrapper<DepartmentDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(DepartmentDO::getName, name)
                .ne(DepartmentDO::getId, toSqlId(existing.getId()));
        if (existing.getTenantId() == null) {
            wrapper.isNull(DepartmentDO::getTenantId);
        } else {
            wrapper.eq(DepartmentDO::getTenantId, toSqlId(existing.getTenantId()));
        }
        if (parentId == null) {
            wrapper.isNull(DepartmentDO::getParentId);
        } else {
            wrapper.eq(DepartmentDO::getParentId, toSqlId(parentId));
        }
        return departmentMapper.selectCount(wrapper) > 0;
    }

    private String toSqlId(Long value) {
        return value == null ? null : value.toString();
    }

    private Long resolveCurrentUserId(Long fallback) {
        try {
            String userId = userHolder.getCurrentUser().getId();
            if (StringUtils.hasText(userId)) {
                return Long.valueOf(userId);
            }
        } catch (Exception e) {
            log.debug("未获取到当前用户ID，保留原更新人", e);
        }
        return fallback;
    }

    private void syncOrganizationUnit(DepartmentDO existing,
                                      DepartmentDO parent,
                                      String name,
                                      Integer displayOrder,
                                      boolean parentIdChanged,
                                      LocalDateTime updatedAt,
                                      Long updatedBy) {
        Long unitId = departmentMapper.selectUnitIdByBusNo(
                existing.getBusNo(),
                toSqlId(existing.getTenantId())
        );
        if (unitId == null) {
            log.warn("科室缺少关联组织单元，跳过组织单元同步，科室ID: {}", existing.getId());
            return;
        }

        Long parentUnitId = null;
        if (parentIdChanged && parent != null) {
            parentUnitId = departmentMapper.selectUnitIdByBusNo(
                    parent.getBusNo(),
                    toSqlId(existing.getTenantId())
            );
            if (parentUnitId == null) {
                throw new IllegalArgumentException("指定的上级科室缺少关联组织单元，无法调整上级");
            }
        }

        int updated = departmentMapper.updateUnitByCode(
                existing.getBusNo(),
                name,
                displayOrder,
                parentIdChanged,
                parentUnitId,
                updatedAt,
                updatedBy,
                toSqlId(existing.getTenantId())
        );
        if (updated != 1) {
            throw new IllegalStateException("关联组织单元更新失败");
        }
    }
}

