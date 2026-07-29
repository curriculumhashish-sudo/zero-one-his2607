package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.DispensingRejectDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.j7.pharmacy.InpatientPharmacyApis;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.j7.pharmacy.SummaryVO;
import com.zeroone.star.pharmacy.service.IInpatientPharmacyService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：住院药房控制器 - 处理住院发药驳回和住院退药确认
 * </p>
 * <p>
 * 功能说明：提供住院药房相关功能的 RESTful API 接口，包括：
 * 1. 住院发药驳回：获取汇总单队列 → 查看药品项目 → 发药核对 → 驳回发药
 * 2. 住院退药确认：查询退药患者队列 → 查看退药明细 → 确认退药（回补库存）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@RestController
@RequestMapping("/inpatient")
@Tag(name = "住院药房", description = "住院发药驳回与退药确认接口")
public class InpatientController implements InpatientPharmacyApis {

    @Resource
    private IInpatientPharmacyService inpatientPharmacyService;

    // ==================== 住院发药驳回流程 ====================
    // 步骤：获取汇总单队列 → 查看药品项目 → 发药核对截单 → 驳回发药单据

    @Operation(summary = "获取汇总单队列", description = "查询所有待发药的汇总单列表，按创建时间倒序排列")
    @GetMapping("/summary-list")
    @Override
    public JsonVO<List<SummaryVO>> getSummaryList() {
        List<SummaryVO> list = inpatientPharmacyService.getSummaryList();
        return JsonVO.success(list);
    }

    @Operation(summary = "根据汇总单ID获取患者药品项目", description = "根据汇总单ID查询该汇总单下的所有发药单据及药品明细")
    @GetMapping("/drug-items")
    @Override
    public JsonVO<List<DrugItemVO>> getDrugItemsBySummary(@RequestParam("summaryId") String summaryId) {
        List<DrugItemVO> list = inpatientPharmacyService.getDrugItemsBySummary(summaryId);
        return JsonVO.success(list);
    }

    @Operation(summary = "执行发药核对（发药后截单）", description = "更新发药单据状态为已发药，完成发药核对截单操作")
    @PostMapping("/confirm-dispensing")
    @Override
    public JsonVO<String> confirmDispensing(@RequestParam("dispensingId") String dispensingId) {
        inpatientPharmacyService.confirmDispensing(dispensingId);
        return JsonVO.success("发药核对成功");
    }

    @Operation(summary = "驳回患者发药单据", description = "驳回指定患者的发药单据，并同步更新对应汇总单状态为已驳回")
    @PostMapping("/reject-dispensing")
    @Override
    public JsonVO<String> rejectDispensing(@RequestBody DispensingRejectDTO dto) {
        inpatientPharmacyService.rejectDispensing(dto);
        return JsonVO.success("发药驳回成功");
    }

    // ==================== 住院退药确认流程 ====================
    // 步骤：查询退药患者队列 → 查看退药明细 → 确认退药（回补库存）

    @Operation(summary = "分页查询退药患者队列", description = "分页查询住院退药患者队列，支持按患者姓名模糊查询")
    @GetMapping("/return-queue")
    @Override
    public JsonVO<PageDTO<PatientQueueVO>> queryReturnPatientQueue(PatientQueueQuery query) {
        PageDTO<PatientQueueVO> result = inpatientPharmacyService.queryReturnPatientQueue(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取患者退药明细信息", description = "根据退药申请单ID获取该患者的退药药品明细列表")
    @GetMapping("/return-drug-items")
    @Override
    public JsonVO<List<DrugItemVO>> getReturnDrugItems(@RequestParam("returnId") String returnId) {
        List<DrugItemVO> list = inpatientPharmacyService.getReturnDrugItems(returnId);
        return JsonVO.success(list);
    }

    @Operation(summary = "确认退药（对勾选药品执行退药，同步回补药品库存）",
            description = "确认退药操作：更新退药单状态为已确认，并对勾选的药品回补库存")
    @PostMapping("/confirm-return")
    @Override
    public JsonVO<String> confirmReturn(@RequestBody ReturnConfirmDTO dto) {
        inpatientPharmacyService.confirmReturn(dto);
        return JsonVO.success("退药确认成功");
    }
}
