package com.zeroone.star.project.j5.emr;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j5.emr.MedicalTemplateQuery;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateDetailVO;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * 病历模板API接口
 */
@Tag(name = "配置管理-病历模板", description = "病历模板配置管理接口")
@RequestMapping("/medical-template")
public interface MedicalTemplateApis {

    @Operation(summary = "获取病历模板列表", description = "分页查询病历模板，支持模板名称、病历类型筛选")
    @PostMapping("/list")
    JsonVO<PageDTO<MedicalTemplateVO>> getList(@RequestBody MedicalTemplateQuery query);

    @Operation(summary = "获取单条病历模板详情", description = "根据模板ID查询病历模板详细信息")
    @GetMapping("/{id}")
    JsonVO<MedicalTemplateDetailVO> getDetail(
            @Parameter(description = "模板ID", required = true, example = "1816800000000000001")
            @PathVariable("id") String id);
}
