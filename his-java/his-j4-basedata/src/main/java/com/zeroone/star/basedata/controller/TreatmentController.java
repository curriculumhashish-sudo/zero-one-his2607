package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
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
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;
/**
 * <p>
 * 描述：诊疗管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Validated
@RestController
@RequestMapping("/treatment")
@Tag(name = "诊疗管理", description = "诊疗管理接口")
public class TreatmentController implements TreatmentApis {

    @Resource
    private ITreatmentService treatmentService;

    @Operation(summary = "获取选择诊疗列表（条件+分页）", description = "用于其他业务模块选择诊疗项目时的分页查询，返回编码/名称/分类")
    @ApiResponse(description = "分页数据，每条含id/name/categoryCode")
    @GetMapping("/query-select-treatment-list")
    @Override
    public JsonVO<PageDTO<TreatmentSelectVO>> listTreatmentOptions(@Valid TreatmentQuery query) {
        return JsonVO.success(treatmentService.listTreatmentOptions(query));
    }


    @Operation(summary = "获取诊疗列表（条件+分页）", description = "根据筛选条件分页查询诊疗项目，支持按关键词（名称/编码）和分类筛选，不传参数默认返回第一页")
    @ApiResponse(description = "分页结果数据")
    @GetMapping("/query-treatment-list")
    @Override
    public JsonVO<PageDTO<TreatmentListVO>> queryTreatmentList(@Valid TreatmentQuery query) {
        return JsonVO.success(treatmentService.queryTreatmentList(query));
    }


    @Operation(summary = "获取诊疗详情", description = "根据诊疗主键ID查询单条数据完整详情")
    @Parameters({
            @Parameter(name = "id", description = "诊疗数据主键ID", required = true, example = "323001764504670208", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "诊疗详情数据")
    @GetMapping("/get-treatment")
    @Override
    public JsonVO<TreatmentDetailVO> getTreatment(@RequestParam @NotBlank(message = "诊疗ID不能为空") String id) {
        return JsonVO.success(treatmentService.getTreatmentById(id));
    }


    @Operation(summary = "新增诊疗", description = "新增诊疗项目")
    @ApiResponse(description = "无返回值")
    @PostMapping("/add-treatment")
    @Override
    public JsonVO<Void> addTreatment(@Validated @RequestBody TreatmentAddDTO treatmentAddDTO) {
        treatmentService.addTreatment(treatmentAddDTO);
        return JsonVO.success(null);
    }

    @Operation(summary = "修改诊疗", description = "编辑诊疗项目（携带编辑校验码，支持并发编辑保护）")
    @ApiResponse(description = "编辑确认结果")
    @PostMapping("/modify-treatment")
    @Override
    public JsonVO<TreatmentDetailVO> modifyTreatment(@Validated @RequestBody TreatmentEditDTO dto) {
        return JsonVO.success(treatmentService.modifyTreatment(dto));
    }


    @Operation(summary = "删除诊疗（支持批量）", description = "删除诊疗项目")
    @Parameters({
            @Parameter(name = "id", description = "诊疗项目ID", required = true, example = "323001764504670208", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "无返回值")
    @DeleteMapping("/remove-treatment")
    @Override
    public JsonVO<Void> removeTreatment(@RequestParam String id) {
        treatmentService.removeTreatment(id);
        return JsonVO.success(null);
    }


    @Operation(summary = "启/停用诊疗（支持批量）", description = "切换诊疗状态（启用/禁用取反）")
    @ApiResponse(description = "切换后的诊疗状态信息")
    @PutMapping("/modify-treatment-status")
    @Override
    public JsonVO<TreatmentDetailVO> modifyStatus(@Validated @RequestBody TreatmentStatusDTO dto) {
        return JsonVO.success(treatmentService.modifyStatus(dto));
    }
}
