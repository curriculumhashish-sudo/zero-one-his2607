package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.j7.pharmacy.InboundApprovalApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import com.zeroone.star.pharmacy.service.IInboundApprovalService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * G7 入库单审批流转控制器。
 *
 * @author lvv
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j7/inbound-approval")
@Tag(name = "G7 入库单审批流转", description = "撤回审批、审批通过、驳回审批")
public class yaofangrukudan implements InboundApprovalApis {

    @Resource
    private IInboundApprovalService inboundApprovalService;

    /**
     * lvv：撤回处于待审批状态的入库单。
     */
    @Override
    @PostMapping("/withdraw")
    @Operation(summary = "撤回审批", description = "仅原申请人可撤回 PENDING_APPROVAL 状态的入库单；撤回原因必填")
    @ApiResponses(@ApiResponse(responseCode = "200", description = "code=10000成功；9999业务校验失败；9995参数校验失败"))
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
    @Override
    @PostMapping("/approve")
    @Operation(summary = "审批通过", description = "登录审批人通过 PENDING_APPROVAL 状态的入库单，状态变为 APPROVED")
    @ApiResponses(@ApiResponse(responseCode = "200", description = "code=10000成功；9999业务校验失败；9995参数校验失败"))
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
    @Override
    @PostMapping("/reject")
    @Operation(summary = "驳回审批", description = "登录审批人驳回 PENDING_APPROVAL 状态的入库单；驳回原因必填，状态变为 REJECTED")
    @ApiResponses(@ApiResponse(responseCode = "200", description = "code=10000成功；9999业务校验失败；9995参数校验失败"))
    public JsonVO<InboundApprovalVO> rejectApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        try {
            return JsonVO.success(inboundApprovalService.rejectApproval(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
