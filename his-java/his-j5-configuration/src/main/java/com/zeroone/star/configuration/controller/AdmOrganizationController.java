package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IAdmOrganizationService;
import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.ConfigurationManagement.PlaceApis;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class AdmOrganizationController implements PlaceApis {

    @Resource
    private IAdmOrganizationService admOrganizationService;

    @Override
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery) {
        return admOrganizationService.getOrgName(orgNameQuery);
    }

    @Override
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery) {
        return admOrganizationService.queryRunProPage(runProListQuery);
    }
}
