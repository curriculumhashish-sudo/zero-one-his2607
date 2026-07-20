package com.zeroone.star.j7.inboundapproval.controller;

import com.zeroone.star.project.dto.j7.inboundapproval.InboundApprovalDTO;
import com.zeroone.star.project.j7.inboundapproval.InboundApprovalApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.inboundapproval.InboundApprovalVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * G7 入库单审批流转接口控制器。
 * 当前阶段仅提供前后端联调接口文档及模拟响应，不包含数据库操作。
 *
 * @author lvv
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j7/inbound-approval")
@Tag(name = "G7 入库单审批流转", description = "撤回审批、审批通过、驳回审批")
public class InboundApprovalController implements InboundApprovalApis {

    @Override
    @PostMapping("/withdraw")
    @Operation(summary = "撤回审批", description = "申请人撤回处于待审批状态的入库单")
    public JsonVO<InboundApprovalVO> withdrawApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalVO(dto.getBusNo(), "WITHDRAW", "撤回审批成功"));
    }

    @Override
    @PostMapping("/approve")
    @Operation(summary = "审批通过", description = "审批人通过处于待审批状态的入库单")
    public JsonVO<InboundApprovalVO> approveApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalVO(dto.getBusNo(), "AGREE", "审批通过"));
    }

    @Override
    @PostMapping("/reject")
    @Operation(summary = "驳回审批", description = "审批人驳回处于待审批状态的入库单；remark 填写驳回原因")
    public JsonVO<InboundApprovalVO> rejectApproval(@Valid @RequestBody InboundApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalVO(dto.getBusNo(), "REJECT", "驳回审批成功"));
    }
}
