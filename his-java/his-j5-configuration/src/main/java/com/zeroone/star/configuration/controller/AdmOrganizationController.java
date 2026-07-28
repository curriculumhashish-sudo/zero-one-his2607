package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IAdmOrganizationService;
import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
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
@RequestMapping("/adm-organization")
@Tag(name = "配置管理-执行地点", description = "执行地点增删改接口")
public class AdmOrganizationController {

    @Resource
    private IAdmOrganizationService admOrganizationService;

    @Operation(summary = "获取执行科室名称")
    @GetMapping("/getOrgNmae")
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery) {
        return admOrganizationService.getOrgName(orgNameQuery);
    }

    @Operation(summary = "获取执行项目列表")
    @GetMapping("/queryRunProPage")
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery) {
        return admOrganizationService.queryRunProPage(runProListQuery);
    }
}
