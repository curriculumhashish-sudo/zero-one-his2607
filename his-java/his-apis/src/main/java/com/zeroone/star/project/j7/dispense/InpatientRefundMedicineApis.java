package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.List;

/**
 * <p>
 * 描述：住院退药接口（对齐原型：患者队列、退药明细）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
public interface InpatientRefundMedicineApis {

    /** 分页查询退药患者队列 */
    JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(@Valid InpatientRefundPatientQuery query);

    /** 获取患者退药明细 */
    JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(@Valid InpatientRefundDetailQuery query);

    @Operation(summary = "确认退药（对勾选药品执行退药，同步回补药品库存）",
            description = "确认退药操作：更新退药单状态为已确认，并对勾选的药品回补库存")
    @PostMapping("/confirm-return")
    JsonVO<String> confirmReturn(@Valid @RequestBody ReturnConfirmDTO dto);
}
