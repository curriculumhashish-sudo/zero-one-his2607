package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IServiceCatalog;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogAddDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogEditDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogUpdateDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.query.j4.treatment.ServiceCatalogQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogDetailVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogListVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogSelectVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.Parameters;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springdoc.core.annotations.ParameterObject;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

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

@Validated
@RestController
@RequestMapping("/treatment")
@Tag(name = "basedata-activity-definitiona", description = "诊疗管理接口")
public class ActivityDefinitionController implements TreatmentApis {

    @Resource
    private IServiceCatalog treatmentService;

    @Operation(summary = "获取诊疗列表（条件+分页）", description = "根据筛选条件分页查询诊疗项目，支持按关键词（名称/编码）和分类筛选，不传参数默认返回第一页")
    @ApiResponse(description = "分页结果数据")
    @GetMapping("/query-treatment-list")
    @Override
    public JsonVO<PageDTO<ServiceCatalogListVO>> queryTreatmentList(@ParameterObject ServiceCatalogQuery query) {
        return JsonVO.success(treatmentService.queryServiceCatalogList(query));
    }

    @Operation(summary = "获取诊疗列表（条件+分页）", description = "用于其他业务模块选择诊疗项目时的分页查询，返回编码/名称/分类")
    @ApiResponse(description = "分页数据，每条含id/busNo/name/categoryCode")
    @GetMapping("/list-treatment-options")
    @Override
    public JsonVO<PageDTO<ServiceCatalogSelectVO>> listTreatmentOptions(@ParameterObject ServiceCatalogQuery query) {
        return JsonVO.success(treatmentService.listServiceCatalogOptions(query));
    }

    @Operation(summary = "获取诊疗详情", description = "根据诊疗主键ID查询单条数据完整详情")
    @Parameters({
            @Parameter(name = "id", description = "诊疗数据主键ID", required = true, example = "1890000000000000001", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "诊疗详情数据")
    @GetMapping("/get-treatment")
    @Override
    public JsonVO<ServiceCatalogDetailVO> getTreatment(@RequestParam String id) {
        return JsonVO.success(treatmentService.getServiceCatalogById(id));
    }

    @Operation(summary = "获取诊疗分类选项", description = "返回所有诊疗分类下拉选项")
    @ApiResponse(description = "分类名称列表")
    @GetMapping("/list-category-options")
    @Override
    public JsonVO<List<String>> listCategoryOptions() {
        return JsonVO.success(treatmentService.listCategoryOptions());
    }

    @Operation(summary = "新增诊疗项目", description = "新增诊疗项目")
    @ApiResponse(description = "无返回值")
    @PostMapping("/add-treatment")
    @Override
    public JsonVO<Void> addTreatment(@RequestBody @Valid ServiceCatalogAddDTO serviceCatalogAddDTO) {
        treatmentService.addServiceCatalog(serviceCatalogAddDTO);
        return JsonVO.success(null);
    }

    @Operation(summary = "编辑诊疗项目", description = "编辑诊疗项目（携带编辑校验码，支持并发编辑保护）")
    @ApiResponse(description = "编辑确认结果")
    @PostMapping("/modify-treatment")
    @Override
    public JsonVO<ServiceCatalogDetailVO> modifyTreatment(@RequestBody @Valid ServiceCatalogEditDTO dto) {
        return JsonVO.success(treatmentService.modifyServiceCatalog(dto));
    }

    @Operation(summary = "更新诊疗项目", description = "更新诊疗项目（无编辑校验码，直接覆盖更新）")
    @ApiResponse(description = "修改确认结果")
    @PostMapping("/update-treatment")
    @Override
    public JsonVO<ServiceCatalogDetailVO> updateTreatment(@RequestBody @Valid ServiceCatalogUpdateDTO dto) {
        return JsonVO.success(treatmentService.updateServiceCatalog(dto));
    }

    @Operation(summary = "切换诊疗状态", description = "切换诊疗状态（启用/禁用取反）")
    @ApiResponse(description = "切换后的诊疗状态信息")
    @PutMapping("/modify-treatment-status")
    @Override
    public JsonVO<ServiceCatalogDetailVO> modifyStatus(@RequestBody @Valid ServiceCatalogStatusDTO dto) {
        return JsonVO.success(treatmentService.modifyStatus(dto));
    }

    @Operation(summary = "删除诊疗项目", description = "删除诊疗项目")
    @Parameters({
            @Parameter(name = "id", description = "诊疗项目ID", required = true, example = "1890000000000000001", in = ParameterIn.QUERY)
    })
    @ApiResponse(description = "无返回值")
    @DeleteMapping("/remove-treatment")
    @Override
    public JsonVO<Void> removeTreatment(@RequestParam String id) {
        treatmentService.removeServiceCatalog(id);
        return JsonVO.success(null);
    }
}
