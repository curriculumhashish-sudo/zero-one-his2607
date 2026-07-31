package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.pharmacy.service.IOutpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

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
 * 4. 拒绝退药
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
@RequestMapping("/j7/outpatient-return")
@Tag(name = "门诊退药管理", description = "门诊退药业务流程：获取患者队列、退药明细、确认退药、拒绝退药")
public class yaofangtuiyao {

    @Resource
    private IOutpatientPharmacyService outpatientPharmacyService;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 左栏展示待退药/已退药患者列表，支持按状态、日期、患者姓名等条件过滤。
     * 状态为0表示待退药，状态为1表示已退药。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @GetMapping("/queue")
    @Operation(summary = "获取患者队列", description = "分页查询门诊退药患者队列（待退药/已退药）")
    public JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        return JsonVO.success(outpatientPharmacyService.queryOutpatientReturnQueue(query));
    }

    /**
     * 获取患者退药明细信息
     * <p>
     * 点击左栏患者后，中栏展示该患者的退药药品明细，
     * 供药师核对后确认退药或拒绝退药。
     * </p>
     *
     * @param returnId 退药申请单ID
     * @return 药品明细列表
     */
    @GetMapping("/detail/{returnId}")
    @Operation(summary = "获取退药明细", description = "获取患者退药明细信息，供药师核对")
    public JsonVO<List<DrugItemVO>> getReturnDetail(
            @Parameter(description = "退药申请单ID") @PathVariable String returnId) {
        return JsonVO.success(outpatientPharmacyService.getReturnDrugItems(returnId));
    }

    /**
     * 确认退药（回补库存）
     * <p>
     * 药师核对退药明细后，勾选需要退药的药品，点击确认退药。
     * 系统自动回补库存数量，更新退药单据状态为"已完成"。
     * </p>
     *
     * @param dto 退药确认参数（包含退药单ID、患者ID、勾选的药品明细ID列表、操作人）
     * @return 操作结果
     */
    @PostMapping("/confirm")
    @Operation(summary = "确认退药", description = "核对退药明细后确认退药，回补库存数量")
    public JsonVO<String> confirmReturn(@Valid @RequestBody ReturnConfirmDTO dto) {
        try {
            outpatientPharmacyService.confirmReturn(dto);
            return JsonVO.success("确认退药成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    /**
     * 拒绝退药
     * <p>
     * 药师确认药品不符合退药条件时，可以拒绝退药，
     * 需填写拒绝原因。退药单据状态更新为"已拒绝"。
     * </p>
     *
     * @param dto 拒绝退药参数
     * @return 操作结果
     */
    @PostMapping("/reject")
    @Operation(summary = "拒绝退药", description = "拒绝退药并填写拒绝原因")
    public JsonVO<String> rejectReturn(@Valid @RequestBody ReturnRejectDTO dto) {
        try {
            outpatientPharmacyService.rejectReturn(dto);
            return JsonVO.success("拒绝退药成功");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}