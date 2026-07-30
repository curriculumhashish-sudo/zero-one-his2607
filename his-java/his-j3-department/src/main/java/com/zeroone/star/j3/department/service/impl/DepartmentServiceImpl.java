package com.zeroone.star.j3.department.service.impl;

import cn.hutool.core.collection.CollUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
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
import com.zeroone.star.project.exception.j3.department.*;
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

    /**
     *
     * 获取科室树结构
     *
     * @param query 查询参数
     * @return 科室树结构
     * @author 胡傲宇
     */
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
        if (query == null) {
            throw new IllegalArgumentException("查询参数不能为空");
        }

        if (query.getTypeEnum() != null && TypeEnum.getDescByValue(query.getTypeEnum()) == null) {
            throw new IllegalArgumentException("无效的部门类型枚举值: " + query.getTypeEnum());
        }

        if (query.getClassEnum() != null && ClassEnum.getDescByValue(query.getClassEnum()) == null) {
            throw new IllegalArgumentException("无效的部门分类枚举值: " + query.getClassEnum());
        }

        LambdaQueryWrapper<DepartmentDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.like(StrUtil.isNotBlank(query.getKeyword()), DepartmentDO::getName, query.getKeyword())
                .eq(query.getTypeEnum() != null, DepartmentDO::getTypeEnum, TypeEnum.getDescByValue(query.getTypeEnum()))
                .eq(query.getClassEnum() != null, DepartmentDO::getClassEnum, ClassEnum.getDescByValue(query.getClassEnum()))
                .eq(query.getStatusEnum() != null, DepartmentDO::getStatusEnum, query.getStatusEnum())
                .orderByAsc(DepartmentDO::getCreatedAt);

        List<DepartmentDO> allDepartments = departmentMapper.selectList(wrapper);
        if (CollUtil.isEmpty(allDepartments)) {
            return Collections.emptyList();
        }

        Map<Long, DepartmentTreeVO> voMap = new LinkedHashMap<>();
        for (DepartmentDO dept : allDepartments) {
            voMap.put(dept.getId(), dept.toTreeVO());
        }

        Set<Long> missingParentIds = allDepartments.stream()
                .map(DepartmentDO::getParentId)
                .filter(pid -> pid != null && !voMap.containsKey(pid))
                .collect(Collectors.toCollection(LinkedHashSet::new));

        while (!missingParentIds.isEmpty()) {
            List<DepartmentDO> missingDepartments = departmentMapper.selectByIds(missingParentIds);
            if (CollUtil.isEmpty(missingDepartments)) {
                break;
            }
            Set<Long> nextMissing = new LinkedHashSet<>();
            for (DepartmentDO dept : missingDepartments) {
                voMap.putIfAbsent(dept.getId(), dept.toTreeVO());
                if (dept.getParentId() != null && !voMap.containsKey(dept.getParentId())) {
                    nextMissing.add(dept.getParentId());
                }
            }
            missingParentIds = nextMissing;
        }

        List<DepartmentTreeVO> result = new ArrayList<>();
        for (DepartmentTreeVO vo : voMap.values()) {
            if (vo.getParentId() == null) {
                result.add(vo);
            } else {
                DepartmentTreeVO parent = voMap.get(vo.getParentId());
                if (parent != null) {
                    if (parent.getChildren() == null) {
                        parent.setChildren(new ArrayList<>());
                    }
                    parent.getChildren().add(vo);
                }
            }
        }

        return result;
    }

    /**
     * 获取科室详情
     *
     * @param id 科室对象ID
     * @return 科室详情结果
     * @author 胡傲宇
     */
    @Override
    public DepartmentDetailVO getDepartmentDetail(Long id) {
        if (id == null || id <= 0) {
            throw new IllegalArgumentException("部门ID不合法: " + id);
        }

        DepartmentDO departmentDO = departmentMapper.selectById(id);
        if (departmentDO == null) {
            throw new DepartmentNotFoundException("该科室不存在，ID: " + id);
        }
        return departmentDO.toDetailVO();
    }

    /**
     * 新增科室
     *
     * @param dto 新增科室参数DTO
     * @author 胡傲宇
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void createDepartment(DepartmentCreateDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("创建参数不能为空");
        }

        if (StrUtil.isBlank(dto.getName())) {
            throw new IllegalArgumentException("部门名称不能为空");
        }

        if (dto.getTypeEnum() == null || TypeEnum.getDescByValue(dto.getTypeEnum()) == null) {
            throw new IllegalArgumentException("无效的部门类型枚举值");
        }

        DepartmentDO parentDepartment = null;
        if (Objects.equals(TypeEnum.HOSPITAL.getDesc(), TypeEnum.getDescByValue(dto.getTypeEnum()))) {
            if (dto.getParentId() != null) {
                throw new IllegalArgumentException("医院类型的上级科室必须为空");
            }
        } else {
            if (dto.getParentId() == null) {
                throw new EmptyParentDepartmentException("科室的上级科室不能为空");
            }
            parentDepartment = departmentMapper.selectById(dto.getParentId());
            if (parentDepartment == null) {
                throw new DepartmentNotFoundException("该科室的上级科室不存在");
            }
        }

        if (dto.getDefDoctorId() != null && departmentMapper.isDoctorExist(dto.getDefDoctorId()) == 0) {
            throw new DoctorNotFoundException("该医务人员不存在");
        }

        String seqKey = "1:OR";
        String busNo = null;
        if (!departmentMapper.isSeqKeyExist(seqKey)) {
            departmentMapper.createSequence(seqKey);
        }
        departmentMapper.incrementSequence(seqKey);
        Long newValue = departmentMapper.getCurrentValueAfterIncrement();
        if (newValue == null) {
            throw new BusNoGenerationException("序列号生成失败");
        }


        String formattedSeq = String.format("OR%04d", newValue);
        if (Objects.equals(TypeEnum.HOSPITAL.getDesc(), TypeEnum.getDescByValue(dto.getTypeEnum())) || parentDepartment == null) {
            busNo = formattedSeq;
        } else {
            busNo = parentDepartment.getBusNo() + "." + formattedSeq;
        }
        List<DepartmentDO> existingBusNoDepartments = departmentMapper.selectList(new LambdaQueryWrapper<DepartmentDO>()
                .eq(DepartmentDO::getBusNo, busNo));
        if (!existingBusNoDepartments.isEmpty()) {
            throw new IllegalArgumentException("部门编号已存在");
        }

        // TODO: 替换为真实的用户上下文获取
        Long currentUserId = 1L;
        Long tenantId = departmentMapper.getTenantIdByUserId(currentUserId);
        if (tenantId == null) {
            throw new CurrentTenantNotFoundException("无法获取当前用户的租户信息");
        }

        List<DepartmentDO> existingNameDepartments = departmentMapper.selectList(new LambdaQueryWrapper<DepartmentDO>()
                .eq(DepartmentDO::getName, dto.getName()));
        if (!existingNameDepartments.isEmpty()) {
            throw new IllegalArgumentException("部门名称已存在");
        }

        LocalDateTime now = LocalDateTime.now();
        DepartmentDO departmentDO = DepartmentDO.builder()
                .tenantId(tenantId)
                .name(dto.getName())
                .busNo(busNo)
                .parentId(parentDepartment != null ? parentDepartment.getId() : null)
                .typeEnum(TypeEnum.getDescByValue(dto.getTypeEnum()))
                .classEnum(ClassEnum.getDescByValue(dto.getClassEnum()))
                .statusEnum(StatusEnum.ACTIVE.getDesc())
                .activeFlag(true)
                .displayOrder(dto.getDisplayOrder())
                .defDoctorId(dto.getDefDoctorId())
                .pyStr(dto.getPyStr()).wbStr(dto.getWbStr())
                .ybNo(dto.getYbNo()).ybName(dto.getYbName())
                .medinsId(dto.getMedinsId()).medinsAdmdvs(dto.getMedinsAdmdvs())
                .medinsType(dto.getMedinsType()).medinsLv(dto.getMedinsLv())
                .caty(dto.getCaty())
                .createdAt(now).updatedAt(now)
                .createdBy(currentUserId)
                .updatedBy(currentUserId)
                .build();
        departmentMapper.insert(departmentDO);

        Long parentUnitId = null;
        if (parentDepartment != null && StrUtil.isNotBlank(parentDepartment.getBusNo())) {
            parentUnitId = departmentMapper.selectParentUnitIdByBusNo(parentDepartment.getBusNo());
        }

        DepartmentUnitDO unitDO = DepartmentUnitDO.builder()
                .id(IdWorker.getId())
                .code(busNo)
                .displayName(dto.getName())
                .parentId(parentUnitId)
                .sortOrder(dto.getDisplayOrder())
                .tenantId(tenantId)
                .createdAt(now).updatedAt(now)
                .createdBy(currentUserId)
                .updatedBy(currentUserId)
                .build();
        departmentMapper.insertUnit(unitDO);
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

    /**
     * 批量启用或禁用部门
     *
     * @param dto 包含部门ID列表和状态值的DTO
     * @author 胡傲宇
     */
    @Override
    public void activeOrInactiveDepartment(DepartmentBatchDTO dto) {
        // 1. 参数校验
        if (dto == null || CollUtil.isEmpty(dto.getIds())) {
            throw new IllegalArgumentException("部门ID列表不能为空");
        }
        Integer isEnabled = dto.getIsEnabled();
        if (isEnabled == null || (isEnabled != 0 && isEnabled != 1)) {
            throw new IllegalArgumentException("状态值只能为 0(禁用) 或 1(启用)");
        }

        List<Long> ids = dto.getIds();
        boolean enable = (isEnabled == 1);

        if (enable) {
            Set<Long> ancestorIds = new LinkedHashSet<>();
            Set<Long> currentLevel = new HashSet<>(ids);
            while (!currentLevel.isEmpty()) {
                List<DepartmentDO> parents = departmentMapper.selectList(
                        new LambdaQueryWrapper<DepartmentDO>()
                                .in(DepartmentDO::getId, currentLevel)
                                .isNotNull(DepartmentDO::getParentId)
                                .select(DepartmentDO::getParentId));
                Set<Long> nextLevel = parents.stream()
                        .map(DepartmentDO::getParentId)
                        .filter(pid -> !ancestorIds.contains(pid))
                        .collect(Collectors.toSet());
                ancestorIds.addAll(nextLevel);
                currentLevel = nextLevel;
            }
            if (!ancestorIds.isEmpty()) {
                List<DepartmentDO> inactiveAncestors = departmentMapper.selectList(
                        new LambdaQueryWrapper<DepartmentDO>()
                                .in(DepartmentDO::getId, ancestorIds)
                                .eq(DepartmentDO::getActiveFlag, false)
                                .select(DepartmentDO::getId));
                if (!inactiveAncestors.isEmpty()) {
                    throw new DepartmentActiveOrInactiveStatusException(
                            "无法启用部门，存在禁用的上级部门，ID: " + inactiveAncestors.get(0).getId());
                }
            }
        } else {
            Set<Long> descendantIds = new LinkedHashSet<>();
            Queue<Long> queue = new LinkedList<>(ids);
            while (!queue.isEmpty()) {
                Long currentId = queue.poll();
                List<DepartmentDO> children = departmentMapper.selectList(
                        new LambdaQueryWrapper<DepartmentDO>()
                                .eq(DepartmentDO::getParentId, currentId)
                                .select(DepartmentDO::getId));
                for (DepartmentDO child : children) {
                    if (descendantIds.add(child.getId())) {
                        queue.add(child.getId());
                    }
                }
            }
            descendantIds.removeAll(new HashSet<>(ids));
            if (!descendantIds.isEmpty()) {
                List<DepartmentDO> activeDescendants = departmentMapper.selectList(
                        new LambdaQueryWrapper<DepartmentDO>()
                                .in(DepartmentDO::getId, descendantIds)
                                .eq(DepartmentDO::getActiveFlag, true)
                                .select(DepartmentDO::getId));
                if (!activeDescendants.isEmpty()) {
                    throw new DepartmentActiveOrInactiveStatusException(
                            "无法禁用部门，存在启用的下级部门，ID: " + activeDescendants.get(0).getId());
                }
            }
        }

        departmentMapper.update(null, new LambdaUpdateWrapper<DepartmentDO>()
                .in(DepartmentDO::getId, ids)
                .set(DepartmentDO::getStatusEnum, StatusEnum.getDescByValue(isEnabled))
                .set(DepartmentDO::getActiveFlag, enable));
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

