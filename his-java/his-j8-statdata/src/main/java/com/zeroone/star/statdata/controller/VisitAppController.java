package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.j8.statdata.VisitAppApis;
import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.HourTrendVO;
import com.zeroone.star.statdata.service.IVisitTrendService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>描述: 流量统计控制器</p>
 * <p>版权: &copy;01星球</p>
 * <p>地址: 01星球总部</p>
 *
 * @author highxin
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j8/statdata") // 统一模块路径前缀
@Tag(name = "流量统计")
public class VisitAppController implements VisitAppApis {

    private final IVisitTrendService visitTrendService;

    public VisitAppController(IVisitTrendService visitTrendService) {
        this.visitTrendService = visitTrendService;
    }

    @Operation(summary = "获取小时访问趋势",
               description = "根据日期范围获取小时粒度的访问量趋势")
    @GetMapping("/hour-trend")
    @Override
    public JsonVO<HourTrendVO> getHourTrend(HourTrendQuery query) {
        HourTrendVO result = visitTrendService.getHourTrend(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取月访问量",
               description = "根据日期范围获取该月每天的访问量")
    @GetMapping("/month-trend")
    @Override
    public JsonVO<HourTrendVO> getMonthVisitTrend(HourTrendQuery query) {
        HourTrendVO result = visitTrendService.getMonthTrend(query);
        return JsonVO.success(result);
    }
}