package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.AdmOrganizationDO;
import com.zeroone.star.configuration.entity.AdmRunProjectDO;
import com.zeroone.star.configuration.mapper.AdmOrganizationMapper;
import com.zeroone.star.configuration.mapper.AdmRunProjectMapper;
import com.zeroone.star.configuration.service.PlaceService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProAddDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProUpdateDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.query.j5.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.j5.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * 配置管理-执行地点服务实现
 */
@Service
@RequiredArgsConstructor
public class PlaceServiceImpl implements PlaceService {

    private final AdmOrganizationMapper orgMapper;
    private final AdmRunProjectMapper runProMapper;
    private final UserHolder userHolder;

    @Override
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery query) {
        if (!StringUtils.hasText(query.getOrgId())) {
            throw new BusinessException("机构ID不能为空");
        }
        String name = orgMapper.selectNameById(query.getOrgId());
        OrgNameDTO dto = new OrgNameDTO();
        dto.setOrgId(query.getOrgId());
        dto.setOrgName(name);
        return JsonVO.success(dto);
    }

    @Override
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery query) {
        Page<AdmRunProjectDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        LambdaQueryWrapper<AdmRunProjectDO> wrapper = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getItemName())) {
            wrapper.like(AdmRunProjectDO::getItemName, query.getItemName());
        }
        if (StringUtils.hasText(query.getItemCode())) {
            wrapper.eq(AdmRunProjectDO::getItemCode, query.getItemCode());
        }
        if (StringUtils.hasText(query.getExecDeptId())) {
            wrapper.eq(AdmRunProjectDO::getExecDeptId, query.getExecDeptId());
        }
        if (query.getIsEnabled() != null) {
            wrapper.eq(AdmRunProjectDO::getIsEnabled, query.getIsEnabled());
        }
        wrapper.orderByAsc(AdmRunProjectDO::getSortOrder);

        Page<AdmRunProjectDO> result = runProMapper.selectPage(page, wrapper);
        List<RunProListDTO> dtoList = new ArrayList<>();
        for (AdmRunProjectDO record : result.getRecords()) {
            RunProListDTO dto = new RunProListDTO();
            BeanUtils.copyProperties(record, dto);
            dtoList.add(dto);
        }

        PageDTO<RunProListDTO> pageDTO = new PageDTO<>();
        pageDTO.setRows(dtoList);
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setPages(result.getPages());

        return JsonVO.success(pageDTO);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> addRunPro(RunProAddDTO dto) {
        // 校验项目编码是否重复
        Long count = runProMapper.countByItemCode(dto.getItemCode());
        if (count > 0) {
            throw new BusinessException("项目编码已存在");
        }

        AdmRunProjectDO entity = new AdmRunProjectDO();
        BeanUtils.copyProperties(dto, entity);
        if (entity.getIsEnabled() == null) {
            entity.setIsEnabled(1);
        }
        if (entity.getSortOrder() == null) {
            entity.setSortOrder(0);
        }

        // 填充审计字段
        fillAuditFields(entity, true);

        // 如果有科室ID，补充科室名称
        if (StringUtils.hasText(dto.getExecDeptId())) {
            String deptName = orgMapper.selectNameById(dto.getExecDeptId());
            entity.setExecDeptName(deptName);
        }

        runProMapper.insert(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> updateRunPro(RunProUpdateDTO dto) {
        if (!StringUtils.hasText(dto.getId())) {
            throw new BusinessException("ID不能为空");
        }
        AdmRunProjectDO entity = runProMapper.selectById(dto.getId());
        if (entity == null) {
            throw new BusinessException("执行项目不存在");
        }

        // 如果修改了编码，校验是否重复
        if (!entity.getItemCode().equals(dto.getItemCode())) {
            Long count = runProMapper.countByItemCode(dto.getItemCode());
            if (count > 0) {
                throw new BusinessException("项目编码已存在");
            }
        }

        BeanUtils.copyProperties(dto, entity);

        // 如果有科室ID，补充科室名称
        if (StringUtils.hasText(dto.getExecDeptId()) && !dto.getExecDeptId().equals(entity.getExecDeptId())) {
            String deptName = orgMapper.selectNameById(dto.getExecDeptId());
            entity.setExecDeptName(deptName);
        }

        fillAuditFields(entity, false);
        runProMapper.updateById(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<Void> deleteRunPro(String id) {
        AdmRunProjectDO entity = runProMapper.selectById(id);
        if (entity == null) {
            throw new BusinessException("执行项目不存在");
        }
        runProMapper.deleteById(id);
        return JsonVO.success(null);
    }

    private void fillAuditFields(AdmRunProjectDO entity, boolean isInsert) {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setTenantId(currentUser.getTenantId());
                if (isInsert) {
                    entity.setCreatedBy(currentUser.getId());
                }
                entity.setUpdatedBy(currentUser.getId());
            }
        } catch (Exception ignored) {
        }
    }
}
