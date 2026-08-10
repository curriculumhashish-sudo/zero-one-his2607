package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.j7.dispense.InpatientRefundMedicineApis;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.StringJsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailListJsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientPageJsonVO;
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
            content = @Content(schema = @Schema(implementation = InpatientRefundPatientPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"pageIndex\":1,\"pageSize\":10,\"total\":2,\"pages\":1,\"rows\":[{\"id\":\"330317605098229760\",\"patientName\":\"小赵\",\"genderEnum\":\"MALE\",\"genderEnumText\":\"MALE\",\"ageLabel\":\"17岁\",\"wardName\":\"内科疗区\",\"bedName\":\"1号床\",\"hospitalizationNo\":\"ZY2026-06-300002\",\"refundEnum\":\"PENDING_REFUND\",\"refundEnumText\":\"待退药\"},{\"id\":\"330316881840836608\",\"patientName\":\"周女士\",\"genderEnum\":\"FEMALE\",\"genderEnumText\":\"FEMALE\",\"ageLabel\":\"26岁\",\"wardName\":\"内科疗区\",\"bedName\":\"1号床\",\"hospitalizationNo\":\"ZY2026-06-300001\",\"refundEnum\":\"PENDING_REFUND\",\"refundEnumText\":\"待退药\"}]}}")))
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
            content = @Content(schema = @Schema(implementation = InpatientRefundDetailListJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":[{\"id\":\"990000000000000002\",\"dispenseId\":\"990000000000000002\",\"requestId\":\"990000000000000102\",\"itemId\":\"323999010000000013\",\"itemName\":\"白芍\",\"lotNumber\":\"LOT-TEST-002\",\"quantity\":5.00,\"unitCode\":\"10\",\"unitText\":\"g\",\"doctorName\":\"\",\"refundEnum\":\"PENDING_REFUND\",\"refundEnumText\":\"待退药\",\"traceNo\":null,\"selectable\":true},{\"id\":\"990000000000000001\",\"dispenseId\":\"990000000000000001\",\"requestId\":\"990000000000000101\",\"itemId\":\"323999010000000012\",\"itemName\":\"当归\",\"lotNumber\":\"LOT-TEST-001\",\"quantity\":10.00,\"unitCode\":\"10\",\"unitText\":\"g\",\"doctorName\":\"\",\"refundEnum\":\"PENDING_REFUND\",\"refundEnumText\":\"待退药\",\"traceNo\":null,\"selectable\":true}]}")))
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
            content = @Content(schema = @Schema(implementation = StringJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"退药确认成功\"}")))
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
