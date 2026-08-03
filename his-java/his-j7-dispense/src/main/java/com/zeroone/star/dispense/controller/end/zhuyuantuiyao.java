package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.j7.dispense.InpatientRefundMedicineApis;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/j7/refund-medicine")
@Tag(name = "住院退药", description = "住院退药（患者队列、退药明细）")
@Validated
public class zhuyuantuiyao implements InpatientRefundMedicineApis {

    @Resource
    private IInpatientRefundMedicineService inpatientRefundMedicineService;

    @PostMapping("/page-patient-queue")
    @Operation(summary = "分页查询退药患者队列")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    @Override
    public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(
            @RequestBody InpatientRefundPatientQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryPatientQueue(query));
    }

    @PostMapping("/get-refund-detail")
    @Operation(summary = "获取患者退药明细")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    @Override
    public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(
            @RequestBody InpatientRefundDetailQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryRefundDetail(query));
    }

    // ==================== 住院退药确认流程 ====================
    // 步骤：查询退药患者队列 → 查看退药明细 → 确认退药（回补库存）

    @Operation(summary = "确认退药（对勾选药品执行退药，同步回补药品库存）",
            description = "确认退药操作：更新退药单状态为已确认，并对勾选的药品回补库存")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    @PostMapping("/confirm-return")
    @Override
    public JsonVO<String> confirmReturn(@RequestBody ReturnConfirmDTO dto) {
        try {
            inpatientRefundMedicineService.confirmReturn(dto);
            return JsonVO.success("退药确认成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
