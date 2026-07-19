package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDetailDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentOptionDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.Parameters;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springdoc.core.annotations.ParameterObject;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：诊疗管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */

@RestController
@RequestMapping("/treatment")
@Tag(name = "basedata-treatment", description = "诊疗管理接口")
public class TreatmentController implements TreatmentApis {

    @Resource
    private ITreatmentService treatmentService;

    @Operation(summary = "分页查询诊疗项目列表", description = "根据筛选条件分页查询诊疗项目，支持按关键词（名称/编码）和分类筛选，不传参数默认返回第一页")
    @ApiResponse(description = "分页结果数据")
    @GetMapping("/query-treatment-list")
    @Override
    public JsonVO<PageDTO<TreatmentDTO>> queryTreatmentList(@ParameterObject TreatmentQuery query) {
        return JsonVO.success(treatmentService.queryTreatmentList(query));
    }

    @Operation(summary = "查询可选诊疗列表", description = "用于其他业务模块选择诊疗项目时的分页查询，返回编码/名称")
    @ApiResponse(description = "分页数据，每条含id/busNo/name")
    @GetMapping("/list-treatment-options")
    @Override
    public JsonVO<PageDTO<TreatmentSelectDTO>> listTreatmentOptions(@ParameterObject TreatmentSelectQuery query) {
        return JsonVO.success(treatmentService.listTreatmentOptions(query));
    }

    @Operation(summary = "获取诊疗详情", description = "根据诊疗主键ID查询单条数据完整详情")
    @Parameters({
            @Parameter(name = "id", description = "诊疗数据主键ID", required = true, example = "1890000000000000001", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "诊疗详情数据")
    @GetMapping("/get-treatment")
    @Override
    public JsonVO<TreatmentDetailDTO> getTreatment(@RequestParam String id) {
        return JsonVO.success(treatmentService.getTreatmentById(id));
    }

    @Operation(summary = "获取诊疗分类选项", description = "返回所有诊疗分类下拉选项")
    @ApiResponse(description = "分类选项列表")
    @GetMapping("/list-category-options")
    @Override
    public JsonVO<List<TreatmentOptionDTO>> listCategoryOptions() {
        return JsonVO.success(treatmentService.listCategoryOptions());
    }
}
