package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.j7.pharmacy.OutpatientPharmacyApis;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.pharmacy.service.IOutpatientPharmacyService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：门诊药房控制器 - 处理门诊拒绝退药
 * </p>
 * <p>
 * 功能说明：提供门诊药房相关功能的 RESTful API 接口，包括：
 * 1. 查询门诊退药患者队列（分页+条件查询）
 * 2. 查看患者退药明细
 * 3. 执行拒绝退药（更新退药单状态为已拒绝）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@RestController
@RequestMapping("/outpatient")
@Tag(name = "门诊药房", description = "门诊拒绝退药接口")
public class OutpatientController implements OutpatientPharmacyApis {

    @Resource
    private IOutpatientPharmacyService outpatientPharmacyService;

    // ==================== 门诊拒绝退药流程 ====================
    // 步骤：查询退药患者队列 → 查看退药明细 → 执行拒绝退药

    @Operation(summary = "分页查询门诊退药患者队列", description = "分页查询门诊退药患者队列，支持按患者姓名模糊查询")
    @GetMapping("/return-queue")
    @Override
    public JsonVO<PageDTO<PatientQueueVO>> queryOutpatientReturnQueue(PatientQueueQuery query) {
        PageDTO<PatientQueueVO> result = outpatientPharmacyService.queryOutpatientReturnQueue(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取患者退药明细信息", description = "根据退药申请单ID获取该患者的退药药品明细列表")
    @GetMapping("/return-drug-items")
    @Override
    public JsonVO<List<DrugItemVO>> getReturnDrugItems(@RequestParam("returnId") String returnId) {
        List<DrugItemVO> list = outpatientPharmacyService.getReturnDrugItems(returnId);
        return JsonVO.success(list);
    }

    @Operation(summary = "执行拒绝退药", description = "更新退药申请单状态为已拒绝，并记录拒绝原因")
    @PostMapping("/reject-return")
    @Override
    public JsonVO<String> rejectReturn(@RequestBody ReturnRejectDTO dto) {
        outpatientPharmacyService.rejectReturn(dto);
        return JsonVO.success("拒绝退药成功");
    }
}
