package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.OutpatientFeeDetailsApis;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.ExportResultVo;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


@RestController
@RequestMapping("/outpatient")
@Tag(name = "门诊费用明细")
public class OutpatientFeeDetailsController implements OutpatientFeeDetailsApis {

    @Operation(summary = "获取明细列表（条件+分页）")
    @PostMapping("fee-detail")
    @Override
    public JsonVO<OutpatientFeeDetailsVo> queryFeeDetail(FeeDetailQuery query) {

        return null;
    }

    @Operation(summary = "导出报表")
    @GetMapping("fee-detail-export")
    @Override
    public JsonVO<ExportResultVo> exportFeeDetail(FeeDetailQuery query) {
        // Mock 返回导出文件下载URL
        return null;
    }
}

/**
 *
 *
 *
 *
 *
 *

 *
 *
 *
 *
 *
 *
 *
 */