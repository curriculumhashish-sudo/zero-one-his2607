package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawStockOutApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 药库出库审批 前端控制器（桩实现，用于生成Knife4j接口文档）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-告别
 * @since 2026-07-20
 */
@RestController
@Tag(name = "药库出库审批", description = "药库出库审批")
@Validated
public class PharmacyController implements PharmacyApis {

    @PostMapping("/pharmacy/stock-out/submit-approval")
    @Operation(summary = "提交出库审批")
    @Override
    public JsonVO<String> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("提交成功，当前状态：APPROVAL，审批人ID：" + dto.getApproverId());
    }

    @PostMapping("/pharmacy/stock-out/withdraw-approval")
    @Operation(summary = "撤回出库审批")
    @Override
    public JsonVO<String> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("撤回成功，当前状态：WITHDRAW");
    }

    @PostMapping("/pharmacy/stock-out/agree")
    @Operation(summary = "通过出库审批")
    @Override
    public JsonVO<String> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("审批通过，当前状态：AGREE，药房入库壳单号：PI202607200001");
    }

    @PostMapping("/pharmacy/stock-out/reject")
    @Operation(summary = "驳回出库审批")
    @Override
    public JsonVO<String> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("驳回成功，当前状态：REJECT");
    }
}
