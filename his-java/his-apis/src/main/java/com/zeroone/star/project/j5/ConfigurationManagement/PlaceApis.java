package com.zeroone.star.project.j5.ConfigurationManagement;

import com.zeroone.star.project.dto.j5.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProAddDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProUpdateDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j5.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.j5.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * 配置管理-执行地点接口
 */
@Tag(name = "配置管理-执行地点", description = "执行地点增删改接口")
@Validated
@RequestMapping("/adm-organization")
public interface PlaceApis {

    @GetMapping("/getOrgName")
    @Operation(summary = "获取执行科室名称", description = "根据科室id查询名称")
    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery);

    @Operation(summary = "获取执行项目列表", description = "条件+分页查询")
    @GetMapping("/queryRunProPage")
    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery);

    @Operation(summary = "新增执行项目", description = "新增执行项目")
    @PostMapping("/runPro/add")
    JsonVO<String> addRunPro(@Valid @RequestBody RunProAddDTO dto);

    @Operation(summary = "修改执行项目", description = "根据ID修改执行项目")
    @PutMapping("/runPro/update")
    JsonVO<String> updateRunPro(@Valid @RequestBody RunProUpdateDTO dto);

    @Operation(summary = "删除执行项目", description = "根据ID删除执行项目")
    @DeleteMapping("/runPro/delete/{id}")
    JsonVO<Void> deleteRunPro(
            @Parameter(description = "项目ID", required = true)
            @NotBlank(message = "项目ID不能为空")
            @Size(max = 64, message = "项目ID不能超过64个字符")
            @PathVariable("id") String id);
}
