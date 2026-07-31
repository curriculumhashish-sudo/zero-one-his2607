package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.OutpatientFeeDetailsApis;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.ExportResultVo;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;
import com.zeroone.star.report.service.IFeeDetailService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * 门诊费用明细控制器
 * @author J6
 */
@RestController
@RequestMapping("/outpatient")
@Tag(name = "门诊费用明细")
public class OutpatientFeeDetailsController implements OutpatientFeeDetailsApis {

    @Resource
    private IFeeDetailService feeDetailService;

    @Operation(summary = "获取明细列表（条件+分页）")
    @PostMapping("fee-detail")
    @Override
    public JsonVO<OutpatientFeeDetailsVo> queryFeeDetail(@RequestBody FeeDetailQuery query) {
        OutpatientFeeDetailsVo result = feeDetailService.pageFeeDetailQuery(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "导出报表")
    @PostMapping("fee-detail-export")
    @Override
    public JsonVO<ExportResultVo> exportFeeDetail(@RequestBody FeeDetailQuery query) {
        // TODO: 后续实现报表导出功能
        return JsonVO.success(null);
    }
}
