package com.zeroone.star.pharmacy.controller.end;


import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import com.zeroone.star.pharmacy.service.IInboundApprovalService;
import io.swagger.v3.oas.annotations.Operation;
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
    @Operation(summary = "撤回审批", description = "申请人撤回处于待审批状态的入库单")
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
    @Operation(summary = "审批通过", description = "审批人通过处于待审批状态的入库单")
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
    @Operation(summary = "驳回审批", description = "审批人驳回处于待审批状态的入库单，remark 填写驳回原因")
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
    @Operation(summary = "新增入库单据", description = "创建一张新的入库单，状态为草稿。返回入库单主表ID。")
    @PostMapping("/addInStockOrder")
    public JsonVO<String> addInOrder(@RequestBody InStockOrderDTO dto) {
        String orderId = inStockOrderService.create(dto);
        return JsonVO.success(orderId);
    }
    // 保存入库单据（编辑）
    @Operation(summary = "保存入库单据", description = "修改草稿态的入库单，支持对明细的全量增删改。")
    @PutMapping("/{orderId}/saveInStockOrder")
    public JsonVO<String> updateInOrder(@PathVariable String orderId, @RequestBody InStockOrderDTO dto) {
        inStockOrderService.update(orderId, dto);
        return JsonVO.success("保存入库单据成功");
    }
    // 保存并提交审批
    @Operation(summary = "保存并提交审批", description = "保存草稿并立即提交审批，状态变更为“待审批”。")
    @PutMapping("/{orderId}/submitInStockOrder")
    public JsonVO<String> submitInOrder(@PathVariable String orderId, @RequestBody InStockOrderDTO dto) {
        inStockOrderService.submit(orderId, dto);
        return JsonVO.success("保存并提交审批成功");
    }
}
