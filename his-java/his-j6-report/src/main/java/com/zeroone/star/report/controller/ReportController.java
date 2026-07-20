package com.zeroone.star.report.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j6.report.ReportApis;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.FeeDetailVO;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;


@RestController
@RequestMapping("/report")
@Tag(name = "report-center", description = "报表中心")
public class ReportController implements ReportApis {
    @Operation(summary = "门诊费用明细列表（条件+分页）")
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

    @Operation(summary = "门诊费用明细报表导出")
    @GetMapping("fee-detail-export")
    @Override
    public JsonVO<String> exportFeeDetail(FeeDetailQuery query) {
        // Mock 返回导出文件下载URL
        return JsonVO.success("https://example.com/report/export/fee-detail-20260720120000.xlsx");
    }

    @Operation(summary = "获取流量趋势")
    @GetMapping("traffic-trend")
    @Override
    public JsonVO<List<TrafficTrendVO>> queryTrafficTrend(TrafficTrendQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现出参示例
        List<TrafficTrendVO> list = new ArrayList<>();
        list.add(new TrafficTrendVO("2026-07-01", 320L));
        list.add(new TrafficTrendVO("2026-07-02", 450L));
        list.add(new TrafficTrendVO("2026-07-03", 380L));
        list.add(new TrafficTrendVO("2026-07-04", 510L));
        list.add(new TrafficTrendVO("2026-07-05", 600L));
        list.add(new TrafficTrendVO("2026-07-06", 280L));
        list.add(new TrafficTrendVO("2026-07-07", 430L));
        list.add(new TrafficTrendVO("2026-07-08", 520L));
        list.add(new TrafficTrendVO("2026-07-09", 480L));
        list.add(new TrafficTrendVO("2026-07-10", 550L));
        list.add(new TrafficTrendVO("2026-07-11", 390L));
        list.add(new TrafficTrendVO("2026-07-12", 460L));
        list.add(new TrafficTrendVO("2026-07-13", 510L));
        list.add(new TrafficTrendVO("2026-07-14", 580L));
        return JsonVO.success(list);
    }
}
