package com.zeroone.star.report.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j6.report.OutpatientFeeDetailsApis;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.FeeDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;


@RestController
@RequestMapping("/outpatient")
@Tag(name = "门诊费用明细")
public class OutpatientFeeDetailsController implements OutpatientFeeDetailsApis {

    @Operation(summary = "获取明细列表（条件+分页）")
    @GetMapping("fee-detail")
    @Override
    public JsonVO<PageDTO<FeeDetailVO>> queryFeeDetail(FeeDetailQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现分页出参示例
        List<FeeDetailVO> rows = new ArrayList<>();
        rows.add(new FeeDetailVO("MZ202607010001", "张三", "内科", "2026-07-01",
                "药品费", "阿莫西林胶囊", "YP0001", new BigDecimal("2"),
                "李医生", "王收费", "2026-07-01 10:30:00",
                "一级", "城镇职工医保",
                new BigDecimal("25.00"), new BigDecimal("5.00"), new BigDecimal("20.00"),
                new BigDecimal("20.00"), new BigDecimal("12.00"), new BigDecimal("8.00")));
        rows.add(new FeeDetailVO("MZ202607010001", "张三", "内科", "2026-07-01",
                "治疗费", "静脉输液", "ZL0001", new BigDecimal("1"),
                "李医生", "王收费", "2026-07-01 10:31:00",
                "一级", "城镇职工医保",
                new BigDecimal("35.00"), new BigDecimal("0.00"), new BigDecimal("35.00"),
                new BigDecimal("35.00"), new BigDecimal("21.00"), new BigDecimal("14.00")));
        rows.add(new FeeDetailVO("MZ202607020002", "李四", "外科", "2026-07-02",
                "药品费", "头孢拉定胶囊", "YP0002", new BigDecimal("3"),
                "赵医生", "王收费", "2026-07-02 09:15:00",
                "二级", "城乡居民医保",
                new BigDecimal("54.00"), new BigDecimal("10.00"), new BigDecimal("44.00"),
                new BigDecimal("44.00"), new BigDecimal("30.80"), new BigDecimal("13.20")));

        PageDTO<FeeDetailVO> page = new PageDTO<>();
        page.setPageIndex(query.getPageIndex() > 0 ? query.getPageIndex() : 1L);
        page.setPageSize(query.getPageSize() > 0 ? query.getPageSize() : 10L);
        page.setTotal(3L);
        page.setPages(1L);
        page.setRows(rows);
        return JsonVO.success(page);
    }

    @Operation(summary = "导出报表")
    @GetMapping("fee-detail-export")
    @Override
    public JsonVO<String> exportFeeDetail(FeeDetailQuery query) {
        // Mock 返回导出文件下载URL
        return JsonVO.success("https://example.com/report/export/fee-detail-20260720120000.xlsx");
    }
}
