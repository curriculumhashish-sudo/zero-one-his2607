package com.zeroone.star.ons.controller;

import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.ConfigurationManagement.ConfigurationManagementApis;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.annotations.ApiOperation;
import org.apache.poi.ss.formula.functions.T;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 管理域 机构 前端控制器
 * </p>
 *
 * @author erji
 * @since 2026-07-20
 */
@RestController
@RequestMapping("/ons/adm-organization")
public class AdmOrganizationController implements ConfigurationManagementApis {

    @ApiOperation(value = "获取执行科室名称")
    @GetMapping("/getOrgNmae")
    @Override
    public JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery) {
        return null;
    }

    @ApiOperation(value = "获取执行项目列表")
    @GetMapping("/queryRunProPage")
    @Override
    public JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery) {
        return null;
    }




    // 获取执行项目列表（条件+分页）
}
