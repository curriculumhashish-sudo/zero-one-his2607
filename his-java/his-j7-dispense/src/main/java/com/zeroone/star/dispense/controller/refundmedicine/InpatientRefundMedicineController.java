package com.zeroone.star.dispense.controller.refundmedicine;

import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientRefundConfirmDTO;
import com.zeroone.star.project.j7.dispense.InpatientRefundMedicineApis;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundConfirmVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundInitVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigDecimal;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 描述：住院退药控制器（loadInit 真实查询；其余接口暂返回假数据）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
@RestController
@RequestMapping("/api/j7/refund-medicine")
@Tag(name = "住院退药", description = "住院退药（初始化、患者队列、退药明细、确认退药）")
@Validated
public class InpatientRefundMedicineController implements InpatientRefundMedicineApis {

    @Resource
    private IInpatientRefundMedicineService inpatientRefundMedicineService;

    @PostMapping("/load-init")
    @Operation(summary = "加载页面初始化选项")
    @Override
    public JsonVO<InpatientRefundInitVO> loadInit() {
        return JsonVO.success(inpatientRefundMedicineService.loadInit());
    }

    @PostMapping("/page-patient-queue")
    @Operation(summary = "分页查询退药患者队列")
    @Override
    public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(
            @Validated @RequestBody InpatientRefundPatientQuery query) {
        InpatientRefundPatientVO row = new InpatientRefundPatientVO();
        row.setEncounterId("329874660147597312");
        row.setPatientName("张三");
        row.setGenderEnum("1");
        row.setGenderEnumText("男");
        row.setAge("45岁");
        row.setWardName("内科病区");
        row.setBedName("08床");
        row.setEncounterNo("ZY20260719001");
        boolean pending = query.getQueueTab() == null || "PENDING".equalsIgnoreCase(query.getQueueTab());
        row.setRefundEnum(pending ? "PENDING_REFUND" : "REFUNDED");
        row.setRefundEnumText(pending ? "待退药" : "已退药");

        PageDTO<InpatientRefundPatientVO> page = new PageDTO<>();
        page.setPageIndex(query.getPageIndex() > 0 ? query.getPageIndex() : 1L);
        page.setPageSize(query.getPageSize() > 0 ? query.getPageSize() : 10L);
        page.setTotal(1L);
        page.setPages(1L);
        page.setRows(Collections.singletonList(row));
        return JsonVO.success(page);
    }

    @PostMapping("/get-refund-detail")
    @Operation(summary = "获取患者退药明细")
    @Override
    public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(
            @Validated @RequestBody InpatientRefundDetailQuery query) {
        boolean pending = query.getRefundStatus() == null
                || "PENDING_REFUND".equalsIgnoreCase(query.getRefundStatus());

        InpatientRefundDetailVO detail = new InpatientRefundDetailVO();
        detail.setDispenseId("329906419476205568");
        detail.setRequestId("329891428408758272");
        detail.setItemId("322997883812974592");
        detail.setItemName("阿莫西林胶囊");
        detail.setLotNumber("20260501");
        detail.setQuantity(new BigDecimal("1.00"));
        detail.setUnitCode("8");
        detail.setUnitText("盒");
        detail.setDoctorName("王医生");
        detail.setRefundEnum(pending ? "PENDING_REFUND" : "REFUNDED");
        detail.setRefundEnumText(pending ? "待退药" : "已退药");
        detail.setTraceNo("TRACE-001");
        detail.setSelectable(pending);
        return JsonVO.success(Collections.singletonList(detail));
    }

    @PostMapping("/confirm-return")
    @Operation(summary = "确认退药（假数据）")
    @Override
    public JsonVO<InpatientRefundConfirmVO> confirmReturn(
            @Validated @RequestBody InpatientRefundConfirmDTO dto) {
        int count = dto.getItems() == null ? 0 : dto.getItems().size();
        return JsonVO.success(new InpatientRefundConfirmVO(true, "确认退药成功，共 " + count + " 条"));
    }
}
