package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthTrendDTO;
import com.zeroone.star.project.j8.statdata.VisitAppApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

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

    @Operation(summary = "获取小时访问趋势", 
               description = "根据日期获取当天0-23时每个小时的访问量，返回24条数据")
    @GetMapping("/hour-trend/{day}")
    @Override
    public JsonVO<List<HourTrendDTO>> getHourTrend(
            @Parameter(description = "统计日期，格式：yyyy-MM-dd", 
                       example = "2026-07-20", 
                       required = true)
            @PathVariable String day) {
        return null;
    }

    @Operation(summary = "获取月访问趋势", 
               description = "根据月份获取该月每天的访问量")
    @GetMapping("/month-trend/{month}")
    @Override
    public JsonVO<List<MonthTrendDTO>> getMonthVisitTrend(
            @Parameter(description = "统计月份，格式：yyyy-MM", 
                       example = "2026-07", 
                       required = true)
            @PathVariable String month) {
        return null;
    }
}