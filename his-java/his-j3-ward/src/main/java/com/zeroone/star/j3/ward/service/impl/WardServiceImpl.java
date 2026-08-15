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

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

@Slf4j
@Service
public class WardServiceImpl implements WardService {

    private static final String FORM_WARD = "WARD";
    private static final String FORM_ROOM = "HOUSE";
    private static final String FORM_BED = "BED";
    private static final String STATUS_ENABLED = "ACTIVE";
    private static final String STATUS_DISABLED = "INACTIVE";
    private static final String MODE_INPATIENT = "INPATIENT";
    private static final String OPERATIONAL_OPEN = "OPEN";
    private static final Pattern BED_NAME_PATTERN = Pattern.compile("^\\d+号床$");
    private static final Pattern ROOM_NAME_PATTERN = Pattern.compile("^\\d+$");

    @Resource
    private AdmLocationMapper admLocationMapper;

    @Resource
    private AdmOrganizationMapper admOrganizationMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    public List<WardTreeVO> getWardTree(WardTreeQuery query) {
        if (query == null) {
            query = new WardTreeQuery();
        }
        validatePage(query.getPageIndex(), query.getPageSize());
        String tenantId = getCurrentTenantId();

        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false);

        if (hasText(query.getOrgName())) {
            List<String> organizationIds = selectOrganizationIdsByName(query.getOrgName(), tenantId);
            if (organizationIds.isEmpty()) {
                return Collections.emptyList();
            }
            wrapper.in(AdmLocation::getOrganizationId, organizationIds);
        }

        List<AdmLocation> allLocations = admLocationMapper.selectList(wrapper);
        Map<String, List<AdmLocation>> parentMap = allLocations.stream()
                .filter(item -> hasText(item.getParentId()))
                .collect(Collectors.groupingBy(AdmLocation::getParentId));

        final WardTreeQuery finalQuery = query;
        List<WardTreeVO> filteredRoots = allLocations.stream()
                .filter(item -> !hasText(item.getParentId()))
                .map(root -> buildFilteredTreeNode(root, parentMap, finalQuery))
                .filter(item -> item != null)
                .collect(Collectors.toList());

        int pageIndex = (int) query.getPageIndex();
        int pageSize = (int) query.getPageSize();
        int fromIndex = (pageIndex - 1) * pageSize;
        if (fromIndex >= filteredRoots.size()) {
            return Collections.emptyList();
        }
        int toIndex = Math.min(fromIndex + pageSize, filteredRoots.size());
        return filteredRoots.subList(fromIndex, toIndex);
    }

    @Override
    public List<OptionVO> getWardList() {
        String tenantId = getCurrentTenantId();
        List<AdmLocation> wards = admLocationMapper.selectList(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getFormEnum, FORM_WARD)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        return wards.stream()
                .map(item -> new OptionVO(item.getId(), item.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addWard(WardAddDTO dto) {
        if (dto == null || !hasText(dto.getWardName())) {
            throw new WardBusinessException("病区名称不能为空");
        }
        if (!hasText(dto.getDepartmentCode())) {
            throw new WardBusinessException("关联科室编号不能为空");
        }

        String tenantId = getCurrentTenantId();
        AdmOrganization organization = admOrganizationMapper.selectOne(new LambdaQueryWrapper<AdmOrganization>()
                .eq(AdmOrganization::getBusNo, dto.getDepartmentCode().trim())
                .eq(AdmOrganization::getTenantId, tenantId)
                .eq(AdmOrganization::getIsDeleted, false));
        if (organization == null) {
            throw new WardBusinessException("未找到关联科室：" + dto.getDepartmentCode());
        }

        String wardName = dto.getWardName().trim();
        assertUniqueName(wardName, FORM_WARD, null, tenantId, null);

        AdmLocation entity = baseLocation(wardName, FORM_WARD, tenantId);
        entity.setBusNo(generateBusNo(FORM_WARD));
        entity.setOrganizationId(organization.getId());
        fillCreateFields(entity);
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateWard(String id, WardDTO dto) {
        String tenantId = getCurrentTenantId();
        AdmLocation existing = getActiveLocation(id, FORM_WARD, tenantId);
        String wardName = dto == null ? null : trimToNull(dto.getWardName());
        if (wardName != null && !wardName.equals(existing.getName())) {
            assertUniqueName(wardName, FORM_WARD, null, tenantId, id);
        }

        AdmLocation entity = new AdmLocation();
        if (wardName != null) {
            entity.setName(wardName);
        }
        if (dto != null && dto.getStatus() != null) {
            entity.setStatusEnum(statusToEnum(dto.getStatus()));
        }
        fillUpdateFields(entity);
        updateByIdAndForm(id, FORM_WARD, tenantId, entity);
    }

    @Override
    @Transactional
    public void deleteWard(String id) {
        String tenantId = getCurrentTenantId();
        getActiveLocation(id, FORM_WARD, tenantId);
        Long childCount = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getParentId, id.trim())
                .eq(AdmLocation::getFormEnum, FORM_ROOM)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        if (childCount > 0) {
            throw new WardBusinessException("该病区下仍有病房，请先删除病房后再删除病区");
        }

        AdmLocation entity = new AdmLocation();
        entity.setId(id.trim());
        entity.setIsDeleted(true);
        fillDeleteFields(entity);
        admLocationMapper.updateById(entity);
    }

    @Override
    public List<OptionVO> getRoomListByWard(String wardId) {
        String tenantId = getCurrentTenantId();
        getActiveLocation(wardId, FORM_WARD, tenantId);
        List<AdmLocation> rooms = admLocationMapper.selectList(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getFormEnum, FORM_ROOM)
                .eq(AdmLocation::getParentId, wardId.trim())
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        return rooms.stream()
                .map(item -> new OptionVO(item.getId(), item.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addRoom(RoomAddDTO dto) {
        if (dto == null || !hasText(dto.getRoomName())) {
            throw new WardBusinessException("病房名称不能为空");
        }
        if (!hasText(dto.getWardCode())) {
            throw new WardBusinessException("所属病区编号不能为空");
        }

        String roomName = dto.getRoomName().trim();
        if (!ROOM_NAME_PATTERN.matcher(roomName).matches()) {
            throw new WardBusinessException("病房名称只能为数字");
        }

        String tenantId = getCurrentTenantId();
        AdmLocation parentWard = admLocationMapper.selectOne(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getBusNo, dto.getWardCode().trim())
                .eq(AdmLocation::getFormEnum, FORM_WARD)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        if (parentWard == null) {
            throw new WardBusinessException("未找到所属病区：" + dto.getWardCode());
        }
        assertUniqueName(roomName, FORM_ROOM, parentWard.getId(), tenantId, null);

        AdmLocation entity = baseLocation(roomName, FORM_ROOM, tenantId);
        entity.setBusNo(generateBusNo(FORM_ROOM));
        entity.setParentId(parentWard.getId());
        entity.setOrganizationId(parentWard.getOrganizationId());
        fillCreateFields(entity);
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateRoom(String id, RoomDTO dto) {
        String tenantId = getCurrentTenantId();
        AdmLocation existing = getActiveLocation(id, FORM_ROOM, tenantId);
        String roomName = dto == null ? null : trimToNull(dto.getRoomName());
        if (roomName != null) {
            if (!ROOM_NAME_PATTERN.matcher(roomName).matches()) {
                throw new WardBusinessException("病房名称只能为数字");
            }
            if (!roomName.equals(existing.getName())) {
                assertUniqueName(roomName, FORM_ROOM, existing.getParentId(), tenantId, id);
            }
        }

        AdmLocation entity = new AdmLocation();
        if (roomName != null) {
            entity.setName(roomName);
        }
        if (dto != null && dto.getStatus() != null) {
            entity.setStatusEnum(statusToEnum(dto.getStatus()));
        }
        fillUpdateFields(entity);
        updateByIdAndForm(id, FORM_ROOM, tenantId, entity);
    }

    @Override
    @Transactional
    public void deleteRoom(String id) {
        String tenantId = getCurrentTenantId();
        getActiveLocation(id, FORM_ROOM, tenantId);
        Long childCount = admLocationMapper.selectCount(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getParentId, id.trim())
                .eq(AdmLocation::getFormEnum, FORM_BED)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        if (childCount > 0) {
            throw new WardBusinessException("该病房下仍有床位，请先删除床位后再删除病房");
        }

        AdmLocation entity = new AdmLocation();
        entity.setId(id.trim());
        entity.setIsDeleted(true);
        fillDeleteFields(entity);
        admLocationMapper.updateById(entity);
    }

    @Override
    public List<BedOptionVO> getBedListByRoom(String roomId) {
        String tenantId = getCurrentTenantId();
        getActiveLocation(roomId, FORM_ROOM, tenantId);
        List<AdmLocation> beds = admLocationMapper.selectList(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getFormEnum, FORM_BED)
                .eq(AdmLocation::getParentId, roomId.trim())
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        return beds.stream()
                .map(item -> new BedOptionVO(
                        item.getId(),
                        item.getName(),
                        STATUS_ENABLED.equals(item.getStatusEnum()) ? 1 : 0))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addBed(BedAddDTO dto) {
        if (dto == null || !hasText(dto.getBedName())) {
            throw new WardBusinessException("床位名称不能为空");
        }
        if (!hasText(dto.getRoomCode())) {
            throw new WardBusinessException("所属病房编号不能为空");
        }

        String bedName = dto.getBedName().trim();
        if (!BED_NAME_PATTERN.matcher(bedName).matches()) {
            throw new WardBusinessException("床位名称格式错误，应为X号床");
        }

        String tenantId = getCurrentTenantId();
        AdmLocation parentRoom = admLocationMapper.selectOne(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getBusNo, dto.getRoomCode().trim())
                .eq(AdmLocation::getFormEnum, FORM_ROOM)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        if (parentRoom == null) {
            throw new WardBusinessException("未找到所属病房：" + dto.getRoomCode());
        }
        assertUniqueName(bedName, FORM_BED, parentRoom.getId(), tenantId, null);

        AdmLocation entity = baseLocation(bedName, FORM_BED, tenantId);
        entity.setBusNo(generateBusNo(FORM_BED));
        entity.setParentId(parentRoom.getId());
        entity.setOrganizationId(parentRoom.getOrganizationId());
        fillCreateFields(entity);
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateBed(String id, BedDTO dto) {
        String tenantId = getCurrentTenantId();
        AdmLocation existing = getActiveLocation(id, FORM_BED, tenantId);
        String bedName = dto == null ? null : trimToNull(dto.getBedName());
        if (bedName != null) {
            if (!BED_NAME_PATTERN.matcher(bedName).matches()) {
                throw new WardBusinessException("床位名称格式错误，应为X号床");
            }
            if (!bedName.equals(existing.getName())) {
                assertUniqueName(bedName, FORM_BED, existing.getParentId(), tenantId, id);
            }
        }

        AdmLocation entity = new AdmLocation();
        if (bedName != null) {
            entity.setName(bedName);
        }
        if (dto != null && dto.getStatus() != null) {
            entity.setStatusEnum(statusToEnum(dto.getStatus()));
        }
        fillUpdateFields(entity);
        updateByIdAndForm(id, FORM_BED, tenantId, entity);
    }

    @Override
    @Transactional
    public void deleteBed(String id) {
        String tenantId = getCurrentTenantId();
        getActiveLocation(id, FORM_BED, tenantId);

        AdmLocation entity = new AdmLocation();
        entity.setId(id.trim());
        entity.setIsDeleted(true);
        fillDeleteFields(entity);
        admLocationMapper.updateById(entity);
    }

    private String getCurrentTenantId() {
        String tenantId = userHolder.getCurrentTenantId();
        if (!hasText(tenantId)) {
            throw new WardBusinessException("未找到当前用户的租户信息");
        }
        return tenantId.trim();
    }

    private List<String> selectOrganizationIdsByName(String orgName, String tenantId) {
        return admOrganizationMapper.selectList(new LambdaQueryWrapper<AdmOrganization>()
                        .like(AdmOrganization::getName, orgName.trim())
                        .eq(AdmOrganization::getTenantId, tenantId)
                        .eq(AdmOrganization::getIsDeleted, false))
                .stream()
                .map(AdmOrganization::getId)
                .collect(Collectors.toList());
    }

    private WardTreeVO buildTreeNode(AdmLocation location, Map<String, List<AdmLocation>> parentMap) {
        WardTreeVO node = toTreeNode(location);
        List<WardTreeVO> children = parentMap.getOrDefault(location.getId(), Collections.emptyList())
                .stream()
                .map(child -> buildTreeNode(child, parentMap))
                .collect(Collectors.toList());
        node.setSubCount(children.size());
        if (!children.isEmpty()) {
            node.setChildren(children);
        }
        return node;
    }

    private WardTreeVO buildFilteredTreeNode(AdmLocation location, Map<String, List<AdmLocation>> parentMap, WardTreeQuery query) {
        boolean currentMatches = matchesQuery(location, query);
        List<WardTreeVO> children = new ArrayList<>();
        for (AdmLocation child : parentMap.getOrDefault(location.getId(), Collections.emptyList())) {
            WardTreeVO childNode = currentMatches
                    ? buildTreeNode(child, parentMap)
                    : buildFilteredTreeNode(child, parentMap, query);
            if (childNode != null) {
                children.add(childNode);
            }
        }
        if (!currentMatches && children.isEmpty()) {
            return null;
        }
        WardTreeVO node = toTreeNode(location);
        node.setSubCount(children.size());
        if (!children.isEmpty()) {
            node.setChildren(children);
        }
        return node;
    }

    private boolean matchesQuery(AdmLocation location, WardTreeQuery query) {
        if (hasText(query.getId()) && !query.getId().trim().equals(location.getId())) {
            return false;
        }
        return !hasText(query.getName())
                || location.getName() != null && location.getName().contains(query.getName().trim());
    }

    private WardTreeVO toTreeNode(AdmLocation location) {
        WardTreeVO node = new WardTreeVO();
        node.setId(location.getId());
        node.setName(location.getName());
        node.setType(toNodeType(location.getFormEnum()));
        node.setStatus(STATUS_ENABLED.equals(location.getStatusEnum()) ? 1 : 0);
        return node;
    }

    private String toNodeType(String formEnum) {
        if (FORM_ROOM.equals(formEnum)) {
            return "room";
        }
        return formEnum == null ? null : formEnum.toLowerCase();
    }

    private AdmLocation baseLocation(String name, String formEnum, String tenantId) {
        return AdmLocation.builder()
                .name(name)
                .formEnum(formEnum)
                .statusEnum(STATUS_ENABLED)
                .tenantId(tenantId)
                .modeEnum(MODE_INPATIENT)
                .operationalEnum(OPERATIONAL_OPEN)
                .isDeleted(false)
                .version(0)
                .build();
    }

    private AdmLocation getActiveLocation(String id, String formEnum, String tenantId) {
        validateId(id, "id");
        AdmLocation location = admLocationMapper.selectOne(new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getId, id.trim())
                .eq(AdmLocation::getFormEnum, formEnum)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
        if (location == null) {
            throw new WardBusinessException("记录不存在或已删除");
        }
        return location;
    }

    private void assertUniqueName(String name, String formEnum, String parentId, String tenantId, String excludeId) {
        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<AdmLocation>()
                .eq(AdmLocation::getName, name)
                .eq(AdmLocation::getFormEnum, formEnum)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false);
        if (parentId == null) {
            wrapper.isNull(AdmLocation::getParentId);
        } else {
            wrapper.eq(AdmLocation::getParentId, parentId);
        }
        if (hasText(excludeId)) {
            wrapper.ne(AdmLocation::getId, excludeId.trim());
        }
        if (admLocationMapper.selectCount(wrapper) > 0) {
            throw new WardBusinessException("名称已存在：" + name);
        }
    }

    private void updateByIdAndForm(String id, String formEnum, String tenantId, AdmLocation entity) {
        admLocationMapper.update(entity, new LambdaUpdateWrapper<AdmLocation>()
                .eq(AdmLocation::getId, id.trim())
                .eq(AdmLocation::getFormEnum, formEnum)
                .eq(AdmLocation::getTenantId, tenantId)
                .eq(AdmLocation::getIsDeleted, false));
    }

    private String statusToEnum(Integer status) {
        if (status == 1) {
            return STATUS_ENABLED;
        }
        if (status == 0) {
            return STATUS_DISABLED;
        }
        throw new WardBusinessException("状态只能为0或1");
    }

    private void validatePage(long pageIndex, long pageSize) {
        if (pageIndex < 1 || pageSize < 1) {
            throw new WardBusinessException("pageIndex和pageSize必须大于0");
        }
    }

    private void validateId(String id, String fieldName) {
        if (!hasText(id)) {
            throw new WardBusinessException(fieldName + "不能为空");
        }
        try {
            if (Long.parseLong(id.trim()) < 0) {
                throw new WardBusinessException(fieldName + "不能为负数");
            }
        } catch (NumberFormatException ignored) {
            // ASSIGN_ID is stored as a string; non-numeric strings are left to the existence check.
        }
    }

    private void fillCreateFields(AdmLocation entity) {
        LocalDateTime now = LocalDateTime.now();
        String operator = getCurrentOperator();
        entity.setCreatedAt(now);
        entity.setUpdatedAt(now);
        entity.setCreatedBy(operator);
        entity.setUpdatedBy(operator);
    }

    private void fillUpdateFields(AdmLocation entity) {
        entity.setUpdatedAt(LocalDateTime.now());
        entity.setUpdatedBy(getCurrentOperator());
    }

    private void fillDeleteFields(AdmLocation entity) {
        LocalDateTime now = LocalDateTime.now();
        String operator = getCurrentOperator();
        entity.setUpdatedAt(now);
        entity.setDeletedAt(now);
        entity.setUpdatedBy(operator);
        entity.setDeletedBy(operator);
    }

    private String getCurrentOperator() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && hasText(currentUser.getUsername())) {
                return currentUser.getUsername().trim();
            }
            if (currentUser != null && hasText(currentUser.getId())) {
                return currentUser.getId().trim();
            }
        } catch (Exception e) {
            log.warn("Failed to read current user for audit fields: {}", e.getMessage());
        }
        return "system";
    }

    private String trimToNull(String value) {
        return hasText(value) ? value.trim() : null;
    }

    private boolean hasText(String value) {
        return value != null && !value.trim().isEmpty();
    }

    private String generateBusNo(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().replace("-", "").substring(0, 16);
    }
}
