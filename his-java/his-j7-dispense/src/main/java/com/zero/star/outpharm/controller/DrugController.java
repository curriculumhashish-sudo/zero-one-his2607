package com.zero.star.outpharm.controller;

import com.zeroone.star.project.j7.outpharm.OutPharmApis;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;
import com.zero.star.outpharm.service.DrugService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.Parameters;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.parameters.RequestBody;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import com.baomidou.mybatisplus.core.metadata.IPage;

@RestController
@RequestMapping("/api/j7/outpharm/drug")
@Validated
@Tag(name = "门诊发药-药品处方", description = "处方药品项目相关接口")
@RequiredArgsConstructor
public class DrugController implements OutPharmApis {

    private final DrugService drugService;

    /* ==================== GET ==================== */

    @Operation(
            summary = "分页查询患者药品项目",
            description = "根据条件分页查询患者的处方药品明细"
    )
    @GetMapping("/items")
    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) {
        IPage<PatientDrugItemVO> page = drugService.pagePatientDrugItems(query);
        if (page == null || page.getRecords().isEmpty()) {
            return JsonVO.fail(null);
        }
        return JsonVO.success(page);
    }

    /* ==================== POST ==================== */

    @Operation(
            summary = "批量发药",
            description = "对多个处方药品执行发药操作"
    )
    @PostMapping("/batch-dispense")
    public JsonVO<String> batchDispenseDrug(
            @RequestBody @Schema(description = "发药请求参数") PatientDrugItemQuery query
    ) {
        return JsonVO.fail("该方法由 DrugService 提供");
    }

    /* ==================== PUT ==================== */

    @Operation(
            summary = "更新药品发药状态",
            description = "修改单条药品的发药状态"
    )
    @PutMapping("/{itemId}/status")
    public JsonVO<String> updateDrugItemStatus(
            @Parameter(description = "药品项目ID", required = true, example = "D123456")
            @PathVariable String itemId,

            @Parameter(description = "状态值（0-待发 1-已发 2-退药）", required = true, example = "1")
            @RequestParam String status
    ) {
        return JsonVO.fail("该方法由 DrugService 提供");
    }

    /* ==================== DELETE ==================== */

    @Operation(
            summary = "作废药品项目",
            description = "作废某条处方药品项目"
    )
    @DeleteMapping("/{itemId}")
    public JsonVO<String> deleteDrugItem(
            @Parameter(description = "药品项目ID", required = true, example = "D123456")
            @PathVariable String itemId
    ) {
        return JsonVO.fail("该方法由 DrugService 提供");
    }

    /* ==================== 接口假实现 ==================== */

    @Operation(
            summary = "分页查询患者队列",
            description = "该方法由 PatientController 提供"
    )
    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) {
        return JsonVO.fail("该方法由 PatientController 提供");
    }
}