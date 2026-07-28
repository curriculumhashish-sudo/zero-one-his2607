package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.query.j7.dispense.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.*;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.util.List;

@RestController
@Tag(name = "住院发退药记录", description = "住院发退药记录基础流程")
public class zhuyuanfatuiyaojilu implements DispenseApis {

    // ========== DispenseApis 接口桩代码 ==========

    @Override public JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(DispensingSummaryQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<List<PatientMedicationVO>> queryPatientMedications(String summaryNo) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<String> verifyDispense(DispenseVerifyDTO verifyDTO) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(InpatientDispenseRecordQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(InpatientDispenseRecordQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(InpatientRefundPatientQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(InpatientRefundDetailQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<DispenseGroupVO>> queryPage(DispenseQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(DispenseQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) { throw new UnsupportedOperationException(); }
}
