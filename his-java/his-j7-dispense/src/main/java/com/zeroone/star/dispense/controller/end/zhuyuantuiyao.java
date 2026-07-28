package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/api/j7/refund-medicine")
@Tag(name = "住院退药", description = "住院退药（患者队列、退药明细）")
@Validated
public class zhuyuantuiyao implements DispenseApis {

    @Resource
    private IInpatientRefundMedicineService inpatientRefundMedicineService;

    @PostMapping("/page-patient-queue")
    @Operation(summary = "分页查询退药患者队列")
    @Override
    public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(
            @Validated @RequestBody InpatientRefundPatientQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryPatientQueue(query));
    }

    @PostMapping("/get-refund-detail")
    @Operation(summary = "获取患者退药明细")
    @Override
    public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(
            @Validated @RequestBody InpatientRefundDetailQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryRefundDetail(query));
    }
}
