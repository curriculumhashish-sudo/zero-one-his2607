package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.AdmOrganization;
import com.zeroone.star.configuration.mapper.AdmOrganizationMapper;
import com.zeroone.star.configuration.service.IAdmOrganizationService;
import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.DataTransCallBack;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

@Service
public class AdmOrganizationServiceImpl extends ServiceImpl<AdmOrganizationMapper, AdmOrganization> implements IAdmOrganizationService {

    @Override
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery) {
        // 1. 构建查询条件：根据id查询，且未被逻辑删除且为激活状态
        LambdaQueryWrapper<AdmOrganization> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmOrganization::getId, orgNameQuery.getId())
                .eq(AdmOrganization::getIsDeleted, false)
                .eq(AdmOrganization::getActiveFlag, true);

        // 2. 执行查询，因为id是唯一的，所以使用selectOne获取单条记录
        AdmOrganization organization = baseMapper.selectOne(wrapper);

        // 3. 将实体数据映射到DTO
        OrgNameDTO dto = new OrgNameDTO();
        if (organization != null) {
            dto.setName(organization.getName());
        }

        // 4. 返回结果
        return JsonVO.success(dto);
    }

    @Override
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery) {
//        LambdaQueryWrapper<AdmOrganization> wrapper = new LambdaQueryWrapper<>();
//        wrapper.eq(AdmOrganization::getIsDeleted, false);
//
//        Page<AdmOrganization> page = baseMapper.selectPage(
//                new Page<>(1, 10), wrapper);
//
//        List<RunProListDTO> dtoList = new ArrayList<>();
//        page.getRecords().forEach(org -> {
//            RunProListDTO dto = new RunProListDTO();
//            dtoList.add(dto);
//        });
//
//        PageDTO<RunProListDTO> pageDTO = new PageDTO<>();
//        pageDTO.setRows(dtoList);
//        pageDTO.setTotal(page.getTotal());
//        pageDTO.setPageIndex(page.getCurrent());
//        pageDTO.setPageSize(page.getSize());
//
//        return JsonVO.success(pageDTO);
        LambdaQueryWrapper<AdmOrganization> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmOrganization::getIsDeleted, false);
        if (StringUtils.hasText(runProListQuery.getLikeName())) {
            wrapper.like(AdmOrganization::getName, runProListQuery.getLikeName());
        }
        if (StringUtils.hasText(runProListQuery.getClassify())) {
            wrapper.eq(AdmOrganization::getClassEnum, runProListQuery.getClassify());
        }

        Page<AdmOrganization> page = baseMapper.selectPage(
                new Page<>(runProListQuery.getPageIndex(), runProListQuery.getPageSize()), wrapper);

        PageDTO<RunProListDTO> pageDTO = PageDTO.create(page, (DataTransCallBack<RunProListDTO, AdmOrganization>) org -> {
            RunProListDTO dto = new RunProListDTO();
            dto.setName(org.getName());
            dto.setClassify(org.getClassEnum());
            return dto;
        });

        return JsonVO.success(pageDTO);
    }
}
