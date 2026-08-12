package com.zeroone.star.pharmacy.controller.end;


import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderCreateDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderSaveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import com.zeroone.star.pharmacy.service.IInboundApprovalService;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.InStockOrderSaveJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundPageJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundPageVO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDeleteDTO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.StringDataJsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.ExampleObject;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * G7 入库单审批流转控制器。
 *
 * @author lvv
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j7/inbound-approval")
@Tag(name = "药房入库单管理", description = "撤回审批、审批通过、驳回审批")
public class yaofangrukudan {

    @Resource
    private IInboundApprovalService inboundApprovalService;

    @Resource
    private InStockOrderService inStockOrderService;

    /**
     * lvv：撤回处于待审批状态的入库单。
     */
    @PostMapping("/withdraw")
    @Operation(summary = "撤回审批", description = "仅原申请人可撤回 APPROVAL（审批中）/ PENDING_APPROVAL（待审批）状态的入库单；撤回原因选填，状态变为 WITHDRAW")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "10000 操作成功；9999 参数错误；9995 业务失败",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = InboundApprovalJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"busNo\":\"PSI2026-06-300001\",\"status\":\"WITHDRAW\",\"message\":\"撤回审批成功\"}}")))
    })
    public JsonVO<InboundApprovalVO> withdrawApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        try {
            return JsonVO.success(inboundApprovalService.withdrawApproval(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * lvv：审批通过处于待审批状态的入库单。
     */
    @PostMapping("/approve")
    @Operation(summary = "审批通过", description = "登录审批人通过 APPROVAL（审批中）/ PENDING_APPROVAL（待审批）状态的入库单，状态变为 AGREE")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "10000 操作成功；9999 参数错误；9995 业务失败",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = InboundApprovalJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"busNo\":\"PSI2026-06-300001\",\"status\":\"AGREE\",\"message\":\"审批通过\"}}")))
    })
    public JsonVO<InboundApprovalVO> approveApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        try {
            return JsonVO.success(inboundApprovalService.approveApproval(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * lvv：驳回处于待审批状态的入库单。
     */
    @PostMapping("/reject")
    @Operation(summary = "驳回审批", description = "登录审批人驳回 APPROVAL（审批中）/ PENDING_APPROVAL（待审批）状态的入库单；驳回原因必填，状态变为 REJECT")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "10000 操作成功；9999 参数错误；9995 业务失败",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = InboundApprovalJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"busNo\":\"PSI2026-06-300001\",\"status\":\"REJECT\",\"message\":\"驳回审批成功\"}}")))
    })
    public JsonVO<InboundApprovalVO> rejectApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        try {
            return JsonVO.success(inboundApprovalService.rejectApproval(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * G5 入库单管理接口定义
     */

    // 新增入库单据
    @Operation(summary = "新增入库单据", description = "创建一张新的入库单，状态为 DRAFT（编辑中）。返回入库单主表ID。")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = InStockOrderSaveJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"322955305339588608\",\"orderNo\":\"PSI2026-06-300001\",\"statusEnum\":\"DRAFT\",\"sourceLocationId\":\"322954949549363200\",\"warehouseId\":\"322955305339588608\",\"projectType\":\"DRUG\",\"supplierId\":322954949549363200,\"inDate\":\"2026-07-30\",\"remark\":\"常规入库\",\"createdBy\":\"user_001\",\"createdAt\":\"2026-07-30 10:15:30\",\"totalPrice\":12500.00,\"items\":[]}}")))
    })
    @PostMapping("/addInStockOrder")
    public JsonVO<InStockOrderSaveDTO> addInOrder(@Valid @RequestBody InStockOrderCreateDTO dto) {
        try {
            InStockOrderSaveDTO result = inStockOrderService.create(dto);
            return JsonVO.success(result);
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    // 保存入库单据（编辑）
    @Operation(summary = "保存入库单据", description = "保存编辑中/待审批状态的入库单，支持对明细的全量增删改；保存成功后状态变为 PENDING_APPROVAL（待审批）。")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StringDataJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"保存入库单据成功\"}")))
    })
    @PutMapping("/{orderId}/saveInStockOrder")
    public JsonVO<String> updateInOrder(@PathVariable String orderId,
                                        @Valid @RequestBody InStockOrderSaveDTO dto) {
        try {
            inStockOrderService.update(orderId, dto);
            return JsonVO.success("保存入库单据成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    // 保存并提交审批
    @Operation(summary = "保存并提交审批", description = "保存草稿并立即提交审批，状态变更为 APPROVAL（审批中）。")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StringDataJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"保存并提交审批成功\"}")))
    })
    @PutMapping("/{orderId}/submitInStockOrder")
    public JsonVO<String> submitInOrder(@PathVariable String orderId, @Valid @RequestBody InStockOrderSaveDTO dto) {
        try {
            inStockOrderService.submit(orderId, dto);
            return JsonVO.success("保存并提交审批成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    // 提交审批（纯状态流转，仅传单据号）
    @Operation(summary = "提交审批", description = "将编辑中或待审批状态的入库单提交审批，同一 bus_no 下主表与明细行整体变更为 APPROVAL（审批中），并写入申请时间。仅需传单据号。")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StringDataJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"提交审批成功\"}")))
    })
    @PutMapping("/{orderId}/submitApproval")
    public JsonVO<String> submitApproval(@PathVariable String orderId) {
        try {
            inStockOrderService.submitByOrderId(orderId);
            return JsonVO.success("提交审批成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * chunmu：获取单据列表（条件+分页）
     */
    @PostMapping("/list")
    @Operation(summary = "获取单据列表", description = "根据条件分页查询药房入库单列表")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = PurchaseInboundPageJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[{\"id\":\"1\",\"busNo\":\"PSI2026-06-300001\",\"statusName\":\"同意\",\"createByName\":\"张三\",\"approverName\":\"李四\",\"applyTime\":\"2026-06-30 16:35:00\",\"transferPath\":\"西药库 → 门诊西药房\",\"totalPrice\":1500.00}],\"total\":1,\"size\":10,\"current\":1,\"pages\":1}}")))
    })
    public JsonVO<PurchaseInboundPageVO> listInboundOrders(@Valid @RequestBody PurchaseInboundQuery query) {
        return JsonVO.success(PurchaseInboundPageVO.from(inStockOrderService.listInboundOrders(query)));
    }

    /**
     * chunmu：获取单据详情
     */
    @GetMapping("/detail/{id}")
    @Operation(summary = "获取单据详情", description = "根据入库单ID查询单据的详细信息，包含主表和明细")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = PurchaseInboundDetailJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"329874660147597312\",\"busNo\":\"PSI2026-06-300001\",\"supplierId\":\"4100000000000000003\",\"sourceLocationId\":\"322954885380706304\",\"purposeLocationId\":\"322954949549363200\",\"inDate\":\"2026-06-30\",\"projectType\":\"DRUG\",\"totalPrice\":1500.00,\"statusEnum\":\"PENDING_APPROVAL\",\"applyTime\":\"2026-06-30T16:35:00\",\"remake\":\"紧急补货\",\"items\":[{\"id\":\"101\",\"itemId\":\"MED_001\",\"itemName\":\"阿莫西林胶囊\",\"spec\":\"0.25g*24粒\",\"lotNumber\":\"20260701\",\"expirationDate\":\"2028-01-01\",\"quantity\":100,\"unit\":\"盒\",\"price\":15.00,\"subTotal\":1500.00}]}}")))
    })
    public JsonVO<PurchaseInboundDetailVO> getInboundDetail(@PathVariable("id") String id) {
        try {
            return JsonVO.success(inStockOrderService.getInboundDetail(id));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * chunmu：保存并提交审批
     */
    @PutMapping("/{orderId}/saveAndSubmit")
    @Operation(summary = "保存并提交审批", description = "保存入库单草稿并立即提交审批，状态变更为 APPROVAL（审批中）")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StringDataJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":\"保存并提交审批成功\"}")))
    })
    public JsonVO<String> saveAndSubmitInOrder(@PathVariable String orderId, @Valid @RequestBody InStockOrderSaveDTO dto) {
        try {
            inStockOrderService.submit(orderId, dto);
            return JsonVO.success("保存并提交审批成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @DeleteMapping("/delete")
    @Operation(summary = "删除入库单据", description = "软删除指定单据号的入库单，仅编辑中(DRAFT)/待审批(PENDING_APPROVAL)状态可删，同时解锁关联的库存锁定记录")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = SupplyDocumentDeleteJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"busNo\":\"PSI2026-06-300001\",\"deletedLines\":3,\"unlockedLocks\":3}}")))
    })
    public JsonVO<SupplyDocumentDeleteVO> deleteInStockOrder(@Valid @RequestBody InStockOrderDeleteDTO dto) {
        try {
            return JsonVO.success(inStockOrderService.deleteOrder(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
