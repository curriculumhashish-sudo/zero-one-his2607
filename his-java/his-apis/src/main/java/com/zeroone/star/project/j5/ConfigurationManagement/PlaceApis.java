package com.zeroone.star.project.j5.ConfigurationManagement;

import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;


@Tag(name = "配置管理-执行地点")
@RequestMapping("/adm-organization")
@Tag(name = "配置管理-执行地点", description = "执行地点增删改接口")
public interface PlaceApis {

    @GetMapping("/getOrgNmae")
    @Operation(summary = "获取执行科室名称", description = "根据科室id查询名称")
    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery);

    @Operation(summary = "获取执行项目列表", description = "条件+分页查询")
    @GetMapping("/queryRunProPage")
    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery);
}
