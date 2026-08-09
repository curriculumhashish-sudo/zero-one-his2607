package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.IOutpatientRefundService;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemListJsonVO;
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
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[],\"total\":0,\"size\":10,\"current\":1,\"pages\":0}}")))
    })
    public JsonVO<IPage<PatientQueueVO>> getPatientQueue(@RequestBody PatientQueueQuery query) {
        return JsonVO.success(outpatientRefundService.queryRefundPatientQueue(query));
    }

    /**
     * 获取患者退药明细信息
     * <p>
     * 点击左栏患者后，中栏展示该患者的退药药品明细，
     * 供药师核对后确认退药。
     * </p>
     *
     * @param query 查询参数（包含患者ID、就诊ID等）
     * @return 药品明细列表
     */
    @PostMapping("/drug-items")
    @Operation(summary = "获取退药明细", description = "获取患者退药明细信息，供药师核对")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = PatientDrugItemListJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":[]}")))
    })
    public JsonVO<List<PatientDrugItemVO>> getRefundDrugItems(@RequestBody PatientDrugItemQuery query) {
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
