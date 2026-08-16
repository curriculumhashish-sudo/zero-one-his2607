package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.IOutpatientRefundService;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemPageJsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueuePageJsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import com.zeroone.star.project.vo.StringJsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：门诊退药控制器
 * </p>
 * <p>
 * 功能说明：实现门诊退药业务流程，包括：
 * 1. 分页查询门诊退药患者队列（待退药/已退药）
 * 2. 获取患者退药明细信息
 * 3. 确认退药（回补库存）
 * </p>
 * <p>
 * 操作流程：
 * 1. 进入「药房 → 门诊退药」
 * 2. 左栏选「待退药」和日期，点击患者
 * 3. 核对中栏退药明细后，点「确认退药」
 * 4. 已退记录可在「已退药」查看
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@RestController
@RequestMapping("/api/j7/outpharm/refund")
@Validated
@Tag(name = "门诊退药", description = "门诊退药基础流程：获取患者队列、退药明细、确认退药")
public class menzhentuiyao {

    @Resource
    private IOutpatientRefundService outpatientRefundService;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 左栏展示待退药/已退药患者列表，支持按状态、日期、患者姓名等条件过滤。
     * 状态为PENDING表示待退药，状态为REFUNDED表示已退药。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @PostMapping("/patient-queue")
    @Operation(summary = "获取退药患者队列", description = "分页查询门诊退药患者队列（待退药/已退药）")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = PatientQueuePageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[{\"patientId\":\"326940849401761792\",\"encounterId\":\"329874660147597312\",\"name\":\"周女士\",\"age\":26,\"genderEnum\":\"FEMALE\",\"phone\":\"13854489876\",\"statusEnum\":\"REFUNDED\",\"dispenseEnum\":\"REFUND\",\"drugCount\":2,\"createdAt\":\"2026-06-29 16:50:19\"}],\"total\":1,\"size\":10,\"current\":1,\"pages\":1}}")))
    })
    public JsonVO<IPage<PatientQueueVO>> getPatientQueue(@RequestBody PatientQueueQuery query) {
        return JsonVO.success(outpatientRefundService.queryRefundPatientQueue(query));
    }

    /**
     * 分页获取患者退药明细信息
     * <p>
     * 点击左栏患者后，中栏分页展示该患者的退药药品明细，
     * 供药师核对后确认退药。
     * </p>
     *
     * @param query 查询参数（包含患者ID、就诊ID、分页参数等）
     * @return 药品明细分页数据
     */
    @PostMapping("/drug-items")
    @Operation(summary = "获取退药明细", description = "分页获取患者退药明细信息，供药师核对")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = PatientDrugItemPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[{\"id\":\"329906419664949248\",\"tenantId\":\"1\",\"busNo\":\"MDP2026-06-290010\",\"statusEnum\":\"DECLINED\",\"dispenseEnum\":\"REFUND\",\"medicationId\":\"323999010000000013\",\"patientId\":\"326940849401761792\",\"encounterId\":\"329874660147597312\",\"medReqId\":\"329906419656560640\",\"quantity\":30.00,\"unitCode\":\"10\",\"dispenseQuantity\":null,\"lotNumber\":\"001-012\",\"preparerId\":null,\"prepareTime\":null,\"practitionerId\":\"308791351298756608\",\"dispenseTime\":\"2026-06-29 16:58:42\",\"dosageInstruction\":null,\"methodCode\":null,\"frequencyCode\":null,\"dose\":null,\"doseUnitCode\":null,\"summaryNo\":null,\"name\":\"白芍\",\"merchandiseName\":\"白芍\",\"categoryCode\":\"4\",\"totalVolume\":\"饮片\",\"comprisedText\":\"饮片\",\"ingredient\":null,\"approvalNumber\":null,\"skinTestFlag\":0,\"injectFlag\":0,\"manufacturerText\":\"安国市中药材市场\",\"antibioticFlag\":0,\"rxFlag\":1,\"doseFormCode\":\"11\",\"packageUnitCode\":\"10\",\"pyStr\":\"BX\"},{\"id\":\"329906419635589120\",\"tenantId\":\"1\",\"busNo\":\"MDP2026-06-290009\",\"statusEnum\":\"REFUNDED\",\"dispenseEnum\":\"REFUND\",\"medicationId\":\"323999010000000012\",\"patientId\":\"326940849401761792\",\"encounterId\":\"329874660147597312\",\"medReqId\":\"329906419623006208\",\"quantity\":30.00,\"unitCode\":\"10\",\"dispenseQuantity\":30.00,\"lotNumber\":\"001-011\",\"preparerId\":null,\"prepareTime\":null,\"practitionerId\":\"308791351298756608\",\"dispenseTime\":\"2026-06-29 16:57:52\",\"dosageInstruction\":null,\"methodCode\":null,\"frequencyCode\":null,\"dose\":null,\"doseUnitCode\":null,\"summaryNo\":null,\"name\":\"当归\",\"merchandiseName\":\"当归\",\"categoryCode\":\"4\",\"totalVolume\":\"饮片\",\"comprisedText\":\"饮片\",\"ingredient\":null,\"approvalNumber\":null,\"skinTestFlag\":0,\"injectFlag\":0,\"manufacturerText\":\"安国市中药材市场\",\"antibioticFlag\":0,\"rxFlag\":1,\"doseFormCode\":\"11\",\"packageUnitCode\":\"10\",\"pyStr\":\"DG\"}],\"total\":2,\"size\":10,\"current\":1,\"pages\":1}}")))
    })
    public JsonVO<IPage<PatientDrugItemVO>> getRefundDrugItems(@RequestBody PatientDrugItemQuery query) {
        return JsonVO.success(outpatientRefundService.queryRefundDrugItems(query));
    }

    /**
     * 确认退药（回补库存）
     * <p>
     * 药师核对退药明细后，勾选需要退药的药品，点击确认退药。
     * 系统自动回补库存数量，更新退药单据状态为"已退药"。
     * </p>
     *
     * @param dto 退药确认参数（包含退药单ID、患者ID、勾选的药品明细ID列表、操作人）
     * @return 操作结果
     */
    @PostMapping("/confirm")
    @Operation(summary = "确认退药", description = "核对退药明细后确认退药，回补库存数量")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StringJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"退药确认成功\"}")))
    })
    public JsonVO<String> confirmRefund(@Valid @RequestBody DispenseVerifyDTO dto) {
        try {
            return JsonVO.success(outpatientRefundService.confirmRefund(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    // ==================== 门诊拒绝退药流程 ====================
    // 步骤：查询退药患者队列 → 查看退药明细 → 执行拒绝退药
    @Operation(summary = "执行拒绝退药", description = "更新退药申请单状态为已拒绝，并记录拒绝原因")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StringJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"拒绝退药成功\"}")))
    })
    @PostMapping("/reject-return")
    public JsonVO<String> rejectReturn(@Valid @RequestBody ReturnRejectDTO dto) {
        try {
            outpatientRefundService.rejectReturn(dto);
            return JsonVO.success("拒绝退药成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
