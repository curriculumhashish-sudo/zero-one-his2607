package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.AdmOrganization;
import com.zeroone.star.configuration.mapper.AdmOrganizationMapper;
import com.zeroone.star.configuration.service.IAdmOrganizationService;
import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class AdmOrganizationServiceImpl extends ServiceImpl<AdmOrganizationMapper, AdmOrganization> implements IAdmOrganizationService {

    @Override
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery) {
        LambdaQueryWrapper<AdmOrganization> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmOrganization::getIsDeleted, false)
               .eq(AdmOrganization::getActiveFlag, true);

        List<AdmOrganization> organizations = baseMapper.selectList(wrapper);
        OrgNameDTO dto = new OrgNameDTO();
        return JsonVO.success(dto);
    }

    @Override
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery) {
        LambdaQueryWrapper<AdmOrganization> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmOrganization::getIsDeleted, false);

        Page<AdmOrganization> page = baseMapper.selectPage(
                new Page<>(1, 10), wrapper);

        List<RunProListDTO> dtoList = new ArrayList<>();
        page.getRecords().forEach(org -> {
            RunProListDTO dto = new RunProListDTO();
            dtoList.add(dto);
        });

        PageDTO<RunProListDTO> pageDTO = new PageDTO<>();
        pageDTO.setRows(dtoList);
        pageDTO.setTotal(page.getTotal());
        pageDTO.setPageIndex(page.getCurrent());
        pageDTO.setPageSize(page.getSize());

        return JsonVO.success(pageDTO);
    }
}
