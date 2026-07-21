package com.zero.star.outpharm.controller;

import com.zeroone.star.project.j7.outpharm.OutPharmApis;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;
import com.zero.star.outpharm.service.PatientService;
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
@RequestMapping("/api/j7/outpharm/patient")
@Validated
@Tag(name = "门诊发药-患者队列", description = "处理患者排队取药相关接口")
@RequiredArgsConstructor
public class PatientController implements OutPharmApis {

    private final PatientService patientService;

    /* ==================== GET ==================== */

    @Operation(
            summary = "分页查询患者队列",
            description = "根据条件分页查询当前等待取药的患者列表"
    )
    @Parameters({
            @Parameter(name = "pageNum", description = "当前页码", required = true, example = "1"),
            @Parameter(name = "pageSize", description = "每页条数", required = true, example = "10")
    })
    @GetMapping("/queue")
    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) {
        IPage<PatientQueueVO> page = patientService.pagePatientQueue(query);
        if (page == null || page.getRecords().isEmpty()) {
            return JsonVO.fail(null);
        }
        return JsonVO.success(page);
    }

    /* ==================== POST ==================== */

    @Operation(
            summary = "患者加入发药队列",
            description = "手动将患者加入发药排队队列"
    )
    @PostMapping("/queue")
    public JsonVO<PatientQueueVO> addPatientToQueue(
            @RequestBody @Schema(description = "患者入队参数") PatientQueueQuery query
    ) {
        return JsonVO.fail("该方法由 PatientService 提供");
    }

    /* ==================== PUT ==================== */

    @Operation(
            summary = "更新患者发药状态",
            description = "修改患者排队或发药状态"
    )
    @PutMapping("/{patientId}/status")
    public JsonVO<String> updatePatientStatus(
            @Parameter(description = "患者ID", required = true, example = "P123456")
            @PathVariable String patientId,

            @Parameter(description = "状态值（0-待发药 1-已发药 2-取消）", required = true, example = "1")
            @RequestParam String status
    ) {
        return JsonVO.fail("该方法由 PatientService 提供");
    }

    /* ==================== DELETE ==================== */

    @Operation(
            summary = "取消患者排队",
            description = "将患者从发药队列中移除"
    )
    @DeleteMapping("/{patientId}")
    public JsonVO<String> cancelPatientQueue(
            @Parameter(description = "患者ID", required = true, example = "P123456")
            @PathVariable String patientId
    ) {
        return JsonVO.fail("该方法由 PatientService 提供");
    }

    /* ==================== 接口假实现 ==================== */

    @Operation(
            summary = "分页查询患者药品项目",
            description = "该方法由 DrugController 提供"
    )
    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) {
        return JsonVO.fail("该方法由 DrugController 提供");
    }
}