package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
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

import jakarta.annotation.Resource;

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

    @Operation(summary = "获取诊疗列表（条件+分页）", description = "根据筛选条件分页查询诊疗项目，支持按关键词（名称/编码）和分类筛选，不传参数默认返回第一页")
    @ApiResponse(description = "分页结果数据")
    @GetMapping("/query-treatment-list")
    @Override
    public JsonVO<PageDTO<TreatmentListVO>> queryTreatmentList(@ParameterObject TreatmentQuery query) {
        return JsonVO.success(treatmentService.queryTreatmentList(query));
    }

    @Operation(summary = "获取诊疗列表（条件+分页）", description = "用于其他业务模块选择诊疗项目时的分页查询，返回编码/名称/分类")
    @ApiResponse(description = "分页数据，每条含id/busNo/name/categoryCode")
    @GetMapping("/list-treatment-options")
    @Override
    public JsonVO<PageDTO<TreatmentSelectVO>> listTreatmentOptions(@ParameterObject TreatmentSelectQuery query) {
        return JsonVO.success(treatmentService.listTreatmentOptions(query));
    }

    @Operation(summary = "获取诊疗详情", description = "根据诊疗主键ID查询单条数据完整详情")
    @Parameters({
            @Parameter(name = "id", description = "诊疗数据主键ID", required = true, example = "1890000000000000001", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "诊疗详情数据")
    @GetMapping("/get-treatment")
    @Override
    public JsonVO<TreatmentDetailVO> getTreatment(@RequestParam String id) {
        return JsonVO.success(treatmentService.getTreatmentById(id));
    }

    @Operation(summary = "获取诊疗分类选项", description = "返回所有诊疗分类下拉选项")
    @ApiResponse(description = "分类名称列表")
    @GetMapping("/list-category-options")
    @Override
    public JsonVO<List<String>> listCategoryOptions() {
        return JsonVO.success(treatmentService.listCategoryOptions());
    }


    @Resource
    private ITreatmentService treatmentService;

    @Override
    public TreatmentResponseDTO addTreatment(TreatmentDTO treatmentDTO) {
        return treatmentService.addTreatment(treatmentDTO);
    }

    @Override
    public TreatmentResponseDTO updateTreatment(TreatmentDTO treatmentDTO) {
        return treatmentService.updateTreatment(treatmentDTO);
    }

    @Override
    public Boolean deleteTreatment(String id) {
        return treatmentService.deleteTreatment(id);
    }

    @Override
    public TreatmentResponseDTO updateStatus(TreatmentStatusDTO statusDTO) {
        return treatmentService.updateStatus(statusDTO);
    }
}