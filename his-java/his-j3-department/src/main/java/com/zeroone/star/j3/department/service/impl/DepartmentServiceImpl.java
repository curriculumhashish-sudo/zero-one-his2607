package com.zeroone.star.j3.department.service.impl;

import cn.hutool.core.collection.CollUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
import com.zeroone.star.j3.department.mapper.DepartmentMapper;
import com.zeroone.star.j3.department.service.DepartmentService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.enums.j3.department.ClassEnum;
import com.zeroone.star.project.enums.j3.department.StatusEnum;
import com.zeroone.star.project.enums.j3.department.TypeEnum;
import com.zeroone.star.project.exception.j3.department.*;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

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

        // TODO: 替换为真实的用户上下文获取
        Long currentUserId = 1L;
        Long tenantId = departmentMapper.getTenantIdByUserId(currentUserId);
        if (tenantId == null) {
            throw new CurrentTenantNotFoundException("无法获取当前用户的租户信息");
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

    /**
     * 批量启用或禁用部门
     *
     * @param dto 包含部门ID列表和状态值的DTO
     * @author 胡傲宇
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
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
}

