package com.zeroone.star.j3.department.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
import com.zeroone.star.j3.department.entity.SequenceDO;
import com.zeroone.star.j3.department.mapper.DepartmentMapper;
import com.zeroone.star.j3.department.service.DepartmentService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.enums.j3.department.ClassEnum;
import com.zeroone.star.project.enums.j3.department.StatusEnum;
import com.zeroone.star.project.enums.j3.department.TypeEnum;
import com.zeroone.star.project.exception.j3.department.DepartmentActiveOrInactiveStatusException;
import com.zeroone.star.project.exception.j3.department.DepartmentNotFoundException;
import com.zeroone.star.project.exception.j3.department.DoctorNotFoundException;
import com.zeroone.star.project.exception.j3.department.EmptyParentDepartmentException;
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
        if (parentId == null && Objects.equals(TypeEnum.getDescByValue(dto.getTypeEnum()), "DEPARTMENT")) {
            throw new EmptyParentDepartmentException("科室的上级科室不能为空");
        }
        String defDoctorId = dto.getDefDoctorId();
        if (!defDoctorId.isEmpty()) {
            if (departmentMapper.isDoctorExist(Long.valueOf(defDoctorId)) == 0) {
                throw new DoctorNotFoundException("该医务人员不存在，ID: " + defDoctorId);
            }
        }
        DepartmentDO parentDepartment = departmentMapper.selectById(parentId);
        if (parentDepartment == null) {
            throw new DepartmentNotFoundException("该科室的上级科室不存在，ID: " + parentId);
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
        String typeEnum = TypeEnum.getDescByValue(dto.getTypeEnum());
        if (typeEnum.equals(TypeEnum.HOSPITAL.getDesc())) {
            busNo = String.format("OR%04d", newValue);
        } else {
            String parentBusNO = parentDepartment.getBusNo();
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

        DepartmentUnitDO departmentUnitDO = DepartmentUnitDO.builder()
                .id(IdWorker.getId())
                .code(busNo)
                .displayName(dto.getName())
                .parentId(parentId)
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
}

