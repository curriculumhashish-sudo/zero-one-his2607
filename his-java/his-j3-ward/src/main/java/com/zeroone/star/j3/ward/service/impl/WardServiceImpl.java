package com.zeroone.star.j3.ward.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.j3.ward.entity.AdmLocation;
import com.zeroone.star.j3.ward.mapper.AdmLocationMapper;
import com.zeroone.star.j3.ward.service.WardService;
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

    @Resource
    private AdmLocationMapper admLocationMapper;

    // ==================== 树形结构 ====================

    @Override
    public List<WardTreeVO> getWardTree(WardTreeQuery query) {
        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getIsDeleted, false);
        if (query.getOrgName() != null && !query.getOrgName().isEmpty()) {
            wrapper.like(AdmLocation::getName, query.getOrgName());
        }
        List<AdmLocation> allLocations = admLocationMapper.selectList(wrapper);

        Map<Long, List<AdmLocation>> parentMap = allLocations.stream()
                .filter(l -> l.getParentId() != null)
                .collect(Collectors.groupingBy(AdmLocation::getParentId));

        List<WardTreeVO> treeList = new ArrayList<>();
        for (AdmLocation location : allLocations) {
            if (location.getParentId() == null) {
                treeList.add(buildTreeNode(location, parentMap));
            }
        }
        return treeList;
    }

    private WardTreeVO buildTreeNode(AdmLocation location, Map<Long, List<AdmLocation>> parentMap) {
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
                .eq(AdmLocation::getIsDeleted, false);
        List<AdmLocation> wards = admLocationMapper.selectList(wrapper);
        return wards.stream()
                .map(w -> new OptionVO(w.getId(), w.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addWard(WardAddDTO dto) {
        AdmLocation entity = AdmLocation.builder()
                .name(dto.getWardName())
                .formEnum("WARD")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("WARD"))
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateWard(Long id, WardDTO dto) {
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
    public void deleteWard(Long id) {
        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    // ==================== 病房管理 ====================

    @Override
    public List<OptionVO> getRoomListByWard(Long wardId) {
        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getFormEnum, "ROOM")
                .eq(AdmLocation::getParentId, wardId)
                .eq(AdmLocation::getIsDeleted, false);
        List<AdmLocation> rooms = admLocationMapper.selectList(wrapper);
        return rooms.stream()
                .map(r -> new OptionVO(r.getId(), r.getName()))
                .collect(Collectors.toList());
    }

    @Override
    @Transactional
    public void addRoom(RoomAddDTO dto) {
        AdmLocation entity = AdmLocation.builder()
                .name(dto.getRoomName())
                .formEnum("ROOM")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("ROOM"))
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateRoom(Long id, RoomDTO dto) {
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
    public void deleteRoom(Long id) {
        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    // ==================== 床位管理 ====================

    @Override
    public List<BedOptionVO> getBedListByRoom(Long roomId) {
        LambdaQueryWrapper<AdmLocation> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmLocation::getFormEnum, "BED")
                .eq(AdmLocation::getParentId, roomId)
                .eq(AdmLocation::getIsDeleted, false);
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
        AdmLocation entity = AdmLocation.builder()
                .name(dto.getBedName())
                .formEnum("BED")
                .statusEnum("ENABLED")
                .busNo(generateBusNo("BED"))
                .build();
        admLocationMapper.insert(entity);
    }

    @Override
    @Transactional
    public void updateBed(Long id, BedDTO dto) {
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
    public void deleteBed(Long id) {
        AdmLocation entity = new AdmLocation();
        entity.setId(id);
        entity.setIsDeleted(true);
        admLocationMapper.updateById(entity);
    }

    // ==================== 私有方法 ====================

    private String generateBusNo(String prefix) {
        return prefix + "_" + System.currentTimeMillis();
    }
}

