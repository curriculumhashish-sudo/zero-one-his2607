package com.zeroone.star.j3.ward.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.j3.ward.entity.AdmLocation;
import com.zeroone.star.j3.ward.entity.AdmOrganization;
import com.zeroone.star.j3.ward.exception.WardBusinessException;
import com.zeroone.star.j3.ward.mapper.AdmLocationMapper;
import com.zeroone.star.j3.ward.mapper.AdmOrganizationMapper;
import com.zeroone.star.j3.ward.service.WardService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j3.ward.BedAddDTO;
import com.zeroone.star.project.dto.j3.ward.BedDTO;
import com.zeroone.star.project.dto.j3.ward.RoomAddDTO;
import com.zeroone.star.project.dto.j3.ward.RoomDTO;
import com.zeroone.star.project.dto.j3.ward.WardAddDTO;
import com.zeroone.star.project.dto.j3.ward.WardDTO;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.j3.ward.BedOptionVO;
import com.zeroone.star.project.vo.j3.ward.OptionVO;
import com.zeroone.star.project.vo.j3.ward.WardTreeVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：病区病房管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Slf4j
@Service
public class WardServiceImpl implements WardService {

    /** 床位名称格式：数字+号床 */
    private static final Pattern BED_NAME_PATTERN = Pattern.compile("^\\d+号床$");
    /** 病房名称格式：纯数字 */
    private static final Pattern ROOM_NAME_PATTERN = Pattern.compile("^\\d+$");

    @Resource
    private AdmLocationMapper admLocationMapper;

    @Resource
    private AdmOrganizationMapper admOrganizationMapper;

    @Resource
    private UserHolder userHolder;

    /**
     * 获取当前用户的租户ID
     */
    private String getCurrentTenantId() {
        String currentUserId = "1";
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && currentUser.getId() != null) {
                currentUserId = currentUser.getId();
            }
        } catch (Exception e) {
            log.warn("获取当前用户失败，使用默认用户ID: {}", e.getMessage());
        }
        String tenantId = admOrganizationMapper.getTenantIdByUserId(currentUserId);
        if (tenantId == null || tenantId.isEmpty()) {
            throw new WardBusinessException("未找到当前用户的租户信息");
        }
        return tenantId;
    }

    // ==================== 树形结构 ====================

    @Override
    public List<WardTreeVO> getWardTree(WardTreeQuery query) {
        String tenantId = getCurrentTenantId();

        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getIsDeleted, false);
        wrapper.eq(AdmLocation::getTenantId, tenantId);

        // name 关键词筛选：对地点名称进行模糊匹配
        if (query.getName() != null && !query.getName().isEmpty()) {
            wrapper.like(AdmLocation::getName, query.getName());
        }

        // orgName 筛选：通过关联科室名称过滤
        if (query.getOrgName() != null && !query.getOrgName().isEmpty()) {
            LambdaQueryWrapper<AdmOrganization> orgWrapper = new LambdaQueryWrapper<>();
            orgWrapper.like(AdmOrganization::getName, query.getOrgName());
            List<AdmOrganization> orgs = admOrganizationMapper.selectList(orgWrapper);
            List<String> orgIds = orgs.stream().map(AdmOrganization::getId).collect(Collectors.toList());
            if (!orgIds.isEmpty()) {
                wrapper.in(AdmLocation::getOrganizationId, orgIds);
            } else {
                return Collections.emptyList();
            }
        }

        List<AdmLocation> allLocations = admLocationMapper.selectList(wrapper);

        // 构建 parentId -> children 映射
        Map<String, List<AdmLocation>> parentMap = allLocations.stream()
                .filter(l -> l.getParentId() != null)
                .collect(Collectors.groupingBy(AdmLocation::getParentId));

        // 找出根节点（parentId 为 null 的节点）
        List<AdmLocation> roots = allLocations.stream()
                .filter(l -> l.getParentId() == null)
                .collect(Collectors.toList());

        // 分页处理：对根节点分页
        int pageIndex = Math.max(1, (int) query.getPageIndex());
        int pageSize = Math.max(1, (int) query.getPageSize());
        int fromIndex = (pageIndex - 1) * pageSize;
        if (fromIndex >= roots.size()) {
            return Collections.emptyList();
        }
        int toIndex = Math.min(fromIndex + pageSize, roots.size());
        List<AdmLocation> pagedRoots = roots.subList(fromIndex, toIndex);

        // 构建树
        List<WardTreeVO> treeList = new ArrayList<>();
        for (AdmLocation root : pagedRoots) {
            treeList.add(buildTreeNode(root, parentMap));
        }
        return treeList;
    }

    private WardTreeVO buildTreeNode(AdmLocation location, Map<String, List<AdmLocation>> parentMap) {
        WardTreeVO vo = new WardTreeVO();
        vo.setId(location.getId());
        vo.setName(location.getName());
        vo.setType(location.getFormEnum() != null ? location.getFormEnum().toLowerCase() : null);
        vo.setStatus("ENABLED".equals(location.getStatusEnum()) ? 1 : 0);
        List<AdmLocation> children = parentMap.getOrDefault(location.getId(), Collections.emptyList());
        vo.setSubCount(children.size());
        if (!children.isEmpty()) {
            List<WardTreeVO> childVos = new ArrayList<>();
            for (AdmLocation child : children) {
                childVos.add(buildTreeNode(child, parentMap));
            }
            vo.setChildren(childVos);
        }
        return vo;
    }

    // ==================== 病区管理 ====================

    @Override
    public List<OptionVO> getWardList() {
        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getFormEnum, "WARD")
                .eq(AdmLocation::getIsDeleted, false)
                .eq(AdmLocation::getTenantId, getCurrentTenantId());
        List<AdmLocation> wards = admLocationMapper.selectList(wrapper);
        return wards.stream()
                .map(w -> new OptionVO(w.getId(), w.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addWard(WardAddDTO dto) {
        if (dto.getWardName() == null || dto.getWardName().isEmpty()) {
            throw new WardBusinessException("病区名称不能为空");
        }
        if (dto.getDepartmentCode() == null || dto.getDepartmentCode().isEmpty()) {
            throw new WardBusinessException("关联科室编号不能为空");
        }

        // 查找关联科室
        LambdaQueryWrapper<AdmOrganization> orgWrapper = new LambdaQueryWrapper<>();
        orgWrapper.eq(AdmOrganization::getBusNo, dto.getDepartmentCode());
        AdmOrganization org = admOrganizationMapper.selectOne(orgWrapper);
        if (org == null) {
            throw new WardBusinessException("未找到关联科室：" + dto.getDepartmentCode());
        }

        String tenantId = org.getTenantId();

        // 病区名称唯一性校验
        Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getName, dto.getWardName())
                .eq(AdmLocation::getFormEnum, "WARD")
                .eq(AdmLocation::getIsDeleted, false)
                .eq(AdmLocation::getTenantId, tenantId));
        if (count > 0) {
            throw new WardBusinessException("病区名称已存在：" + dto.getWardName());
        }

        AdmLocation entity = AdmLocation.builder()
                .name(dto.getWardName())
                .formEnum("WARD")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("WARD"))
                .organizationId(org.getId())
                .tenantId(tenantId)
                .modeEnum("INPATIENT")
                .operationalEnum("OPEN")
                .isDeleted(false)
                .version(0)
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateWard(String id, WardDTO dto) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("病区不存在或已删除");
        }
        if (!"WARD".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是病区记录");
        }

        // 名称唯一性校验（仅当修改了名称时）
        if (dto.getWardName() != null && !dto.getWardName().equals(existing.getName())) {
            Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                    .eq(AdmLocation::getName, dto.getWardName())
                    .eq(AdmLocation::getFormEnum, "WARD")
                    .eq(AdmLocation::getIsDeleted, false)
                    .eq(AdmLocation::getTenantId, existing.getTenantId())
                    .ne(AdmLocation::getId, id));
            if (count > 0) {
                throw new WardBusinessException("病区名称已存在：" + dto.getWardName());
            }
        }

        LambdaUpdateWrapper<AdmLocation> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(AdmLocation::getId, id)
                .eq(AdmLocation::getFormEnum, "WARD");
        AdmLocation entity = new AdmLocation();
        if (dto.getWardName() != null) {
            entity.setName(dto.getWardName());
        }
        if (dto.getStatus() != null) {
            entity.setStatusEnum(dto.getStatus() == 1 ? "ENABLED" : "DISABLED");
        }
        admLocationMapper.update(entity, wrapper);
    }

    @Override
    @Transactional
    public void deleteWard(String id) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("病区不存在或已删除");
        }
        if (!"WARD".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是病区记录");
        }

        // 校验是否存在下级病房
        Long childCount = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getParentId, id)
                .eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getIsDeleted, false));
        if (childCount > 0) {
            throw new WardBusinessException("该病区下仍有" + childCount + "个病房，请先删除病房后再删除病区");
        }

        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    // ==================== 病房管理 ====================

    @Override
    public List<OptionVO> getRoomListByWard(String wardId) {
        // 参数校验
        if (wardId == null || wardId.isEmpty()) {
            throw new WardBusinessException("病区ID不能为空");
        }

        // 验证病区是否存在
        AdmLocation ward = admLocationMapper.selectOne(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getId, wardId)
                .eq(AdmLocation::getFormEnum, "WARD")
                .eq(AdmLocation::getIsDeleted, false));
        if (ward == null) {
            throw new WardBusinessException("病区不存在，ID：" + wardId);
        }

        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getParentId, wardId)
                .eq(AdmLocation::getIsDeleted, false)
                .eq(AdmLocation::getTenantId, getCurrentTenantId());
        List<AdmLocation> rooms = admLocationMapper.selectList(wrapper);
        return rooms.stream()
                .map(r -> new OptionVO(r.getId(), r.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addRoom(RoomAddDTO dto) {
        if (dto.getRoomName() == null || dto.getRoomName().isEmpty()) {
            throw new WardBusinessException("病房名称不能为空");
        }
        if (dto.getWardCode() == null || dto.getWardCode().isEmpty()) {
            throw new WardBusinessException("所属病区编号不能为空");
        }

        // 校验病房名称格式：只能为数字
        if (!ROOM_NAME_PATTERN.matcher(dto.getRoomName()).matches()) {
            throw new WardBusinessException("病房名称只能为数字，例如：302");
        }

        // 查找所属病区
        LambdaQueryWrapper<AdmLocation> parentWrapper = new LambdaQueryWrapper<>();
        parentWrapper.eq(AdmLocation::getBusNo, dto.getWardCode())
                .eq(AdmLocation::getFormEnum, "WARD")
                .eq(AdmLocation::getIsDeleted, false);
        AdmLocation parentWard = admLocationMapper.selectOne(parentWrapper);
        if (parentWard == null) {
            throw new WardBusinessException("未找到所属病区：" + dto.getWardCode());
        }

        String tenantId = parentWard.getTenantId();

        // 同一病区下病房名称唯一性校验
        Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getName, dto.getRoomName())
                .eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getParentId, parentWard.getId())
                .eq(AdmLocation::getIsDeleted, false));
        if (count > 0) {
            throw new WardBusinessException("该病区下病房名称已存在：" + dto.getRoomName());
        }

        AdmLocation entity = AdmLocation.builder()
                .name(dto.getRoomName())
                .formEnum("ROOM")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("ROOM"))
                .parentId(parentWard.getId())
                .tenantId(tenantId)
                .organizationId(parentWard.getOrganizationId())
                .modeEnum("INPATIENT")
                .operationalEnum("OPEN")
                .isDeleted(false)
                .version(0)
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateRoom(String id, RoomDTO dto) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("病房不存在或已删除");
        }
        if (!"ROOM".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是病房记录");
        }

        // 名称校验和唯一性校验
        if (dto.getRoomName() != null) {
            if (!ROOM_NAME_PATTERN.matcher(dto.getRoomName()).matches()) {
                throw new WardBusinessException("病房名称只能为数字，例如：302");
            }
            if (!dto.getRoomName().equals(existing.getName())) {
                Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                        .eq(AdmLocation::getName, dto.getRoomName())
                        .eq(AdmLocation::getFormEnum, "ROOM")
                        .eq(AdmLocation::getParentId, existing.getParentId())
                        .eq(AdmLocation::getIsDeleted, false)
                        .ne(AdmLocation::getId, id));
                if (count > 0) {
                    throw new WardBusinessException("该病区下病房名称已存在：" + dto.getRoomName());
                }
            }
        }

        LambdaUpdateWrapper<AdmLocation> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(AdmLocation::getId, id)
                .eq(AdmLocation::getFormEnum, "ROOM");
        AdmLocation entity = new AdmLocation();
        if (dto.getRoomName() != null) {
            entity.setName(dto.getRoomName());
        }
        if (dto.getStatus() != null) {
            entity.setStatusEnum(dto.getStatus() == 1 ? "ENABLED" : "DISABLED");
        }
        admLocationMapper.update(entity, wrapper);
    }

    @Override
    @Transactional
    public void deleteRoom(String id) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("病房不存在或已删除");
        }
        if (!"ROOM".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是病房记录");
        }

        // 校验是否存在下级床位
        Long childCount = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getParentId, id)
                .eq(AdmLocation::getFormEnum, "BED")
                .eq(AdmLocation::getIsDeleted, false));
        if (childCount > 0) {
            throw new WardBusinessException("该病房下仍有" + childCount + "个床位，请先删除床位后再删除病房");
        }

        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    // ==================== 床位管理 ====================

    @Override
    public List<BedOptionVO> getBedListByRoom(String roomId) {
        // 参数校验
        if (roomId == null || roomId.isEmpty()) {
            throw new WardBusinessException("病房ID不能为空");
        }

        // 验证病房是否存在
        AdmLocation room = admLocationMapper.selectOne(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getId, roomId)
                .eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getIsDeleted, false));
        if (room == null) {
            throw new WardBusinessException("病房不存在，ID：" + roomId);
        }

        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getFormEnum, "BED")
                .eq(AdmLocation::getParentId, roomId)
                .eq(AdmLocation::getIsDeleted, false)
                .eq(AdmLocation::getTenantId, getCurrentTenantId());
        List<AdmLocation> beds = admLocationMapper.selectList(wrapper);
        return beds.stream()
                .map(b -> new BedOptionVO(
                        b.getId(),
                        b.getName(),
                        "ENABLED".equals(b.getStatusEnum()) ? 1 : 0))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addBed(BedAddDTO dto) {
        if (dto.getBedName() == null || dto.getBedName().isEmpty()) {
            throw new WardBusinessException("床位名称不能为空");
        }
        if (dto.getRoomCode() == null || dto.getRoomCode().isEmpty()) {
            throw new WardBusinessException("所属病房编号不能为空");
        }

        // 校验床位名称格式：数字+号床
        if (!BED_NAME_PATTERN.matcher(dto.getBedName()).matches()) {
            throw new WardBusinessException("床位名称格式错误，应为数字+号床，例如：1号床");
        }

        // 查找所属病房
        LambdaQueryWrapper<AdmLocation> parentWrapper = new LambdaQueryWrapper<>();
        parentWrapper.eq(AdmLocation::getBusNo, dto.getRoomCode())
                .eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getIsDeleted, false);
        AdmLocation parentRoom = admLocationMapper.selectOne(parentWrapper);
        if (parentRoom == null) {
            throw new WardBusinessException("未找到所属病房：" + dto.getRoomCode());
        }

        String tenantId = parentRoom.getTenantId();

        // 同一病房下床位名称唯一性校验
        Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getName, dto.getBedName())
                .eq(AdmLocation::getFormEnum, "BED")
                .eq(AdmLocation::getParentId, parentRoom.getId())
                .eq(AdmLocation::getIsDeleted, false));
        if (count > 0) {
            throw new WardBusinessException("该病房下床位名称已存在：" + dto.getBedName());
        }

        AdmLocation entity = AdmLocation.builder()
                .name(dto.getBedName())
                .formEnum("BED")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("BED"))
                .parentId(parentRoom.getId())
                .tenantId(tenantId)
                .organizationId(parentRoom.getOrganizationId())
                .modeEnum("INPATIENT")
                .operationalEnum("OPEN")
                .isDeleted(false)
                .version(0)
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateBed(String id, BedDTO dto) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("床位不存在或已删除");
        }
        if (!"BED".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是床位记录");
        }

        // 名称校验和唯一性校验
        if (dto.getBedName() != null) {
            if (!BED_NAME_PATTERN.matcher(dto.getBedName()).matches()) {
                throw new WardBusinessException("床位名称格式错误，应为数字+号床，例如：1号床");
            }
            if (!dto.getBedName().equals(existing.getName())) {
                Long count = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                        .eq(AdmLocation::getName, dto.getBedName())
                        .eq(AdmLocation::getFormEnum, "BED")
                        .eq(AdmLocation::getParentId, existing.getParentId())
                        .eq(AdmLocation::getIsDeleted, false)
                        .ne(AdmLocation::getId, id));
                if (count > 0) {
                    throw new WardBusinessException("该病房下床位名称已存在：" + dto.getBedName());
                }
            }
        }

        LambdaUpdateWrapper<AdmLocation> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(AdmLocation::getId, id)
                .eq(AdmLocation::getFormEnum, "BED");
        AdmLocation entity = new AdmLocation();
        if (dto.getBedName() != null) {
            entity.setName(dto.getBedName());
        }
        if (dto.getStatus() != null) {
            entity.setStatusEnum(dto.getStatus() == 1 ? "ENABLED" : "DISABLED");
        }
        admLocationMapper.update(entity, wrapper);
    }

    @Override
    @Transactional
    public void deleteBed(String id) {
        // 校验记录存在且未被删除
        AdmLocation existing = admLocationMapper.selectById(id);
        if (existing == null || Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new WardBusinessException("床位不存在或已删除");
        }
        if (!"BED".equals(existing.getFormEnum())) {
            throw new WardBusinessException("该ID对应的不是床位记录");
        }

        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    /**
     * 生成业务编号：前缀 + UUID前16位
     */
    private String generateBusNo(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().replace("-", "").substring(0, 16);
    }
}
