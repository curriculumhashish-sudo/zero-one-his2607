package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IDispensingService;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.j7.dispense.DispensingApis;
import com.zeroone.star.project.query.j7.dispense.DispenseRejectQuery;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.StringJsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryPageJsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationListJsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/j7/dispensing")
@Validated
@Tag(name = "住院发药记录", description = "住院发药记录基础流程")
public class zhuyuanfayao implements DispensingApis {

    @Resource
    private IDispensingService dispensingService;

    @Operation(summary = "获取汇总单队列")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = DispensingSummaryPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"pageIndex\":1,\"pageSize\":10,\"total\":1,\"pages\":1,\"rows\":[{\"summaryNo\":\"HZ20240719001\",\"wardId\":\"loc001\",\"wardName\":\"内科一病区\",\"deptId\":\"org001\",\"deptName\":\"住院药房\",\"totalQty\":50.00,\"patientCount\":5,\"status\":\"PENDING\",\"createTime\":\"2026-07-19 10:30:00\",\"dispenseTime\":null,\"operatorName\":\"药师张三\"}]}}")))
    })
    @PostMapping("query-summary-list")
    @Override
    public JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(@Valid @RequestBody DispensingSummaryQuery query) {
        ExtendPageDTO<DispensingSummaryVO> page = dispensingService.querySummaryList(query);
        return JsonVO.success(page);
    }

    @Operation(summary = "获取患者药品明细")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = PatientMedicationListJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":[{\"id\":\"d001\",\"summaryNo\":\"HZ20240719001\",\"patientId\":\"pat001\",\"patientName\":\"张三\",\"bedNo\":\"01床\",\"medicationId\":\"med001\",\"drugCode\":\"YP0001\",\"drugName\":\"阿莫西林胶囊\",\"spec\":\"0.25g*24粒\",\"unit\":\"盒\",\"quantity\":10.00,\"dispenseQuantity\":null,\"dosageInstruction\":\"口服，每日三次\",\"frequencyCode\":\"TID\",\"lotNumber\":null}]}")))
    })
    @GetMapping("query-patient-medications")
    @Override
    public JsonVO<List<PatientMedicationVO>> queryPatientMedications(
            @Parameter(description = "汇总单号", example = "1") @RequestParam String summaryNo) {
        List<PatientMedicationVO> list = dispensingService.queryPatientMedications(summaryNo);
        return JsonVO.success(list);
    }

    @Operation(summary = "核对发药")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StringJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"发药成功\"}")))
    })
    @PostMapping("verify-dispense")
    @Override
    public JsonVO<String> verifyDispense(@Valid @RequestBody DispenseVerifyDTO verifyDTO) {
        try {
            String result = dispensingService.verifyDispense(verifyDTO);
            return JsonVO.success(result);
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    // =================== 住院发药驳回 ====================
    @Operation(summary = "驳回患者发药单据", description = "驳回指定患者的发药单据，并同步更新对应汇总单状态为已驳回")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StringJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"发药驳回成功\"}")))
    })
    @PostMapping("/reject-dispensing")
    @Override
    public JsonVO<String> rejectDispensing(@Valid @RequestBody DispenseRejectQuery dto) {
        try {
            dispensingService.rejectDispensing(dto);
            return JsonVO.success("发药驳回成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
