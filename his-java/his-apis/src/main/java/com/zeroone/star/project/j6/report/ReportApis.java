package com.zeroone.star.project.j6.report;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.FeeDetailVO;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;

import java.util.List;

public interface ReportApis {
    /**
     * 门诊费用明细列表（条件+分页）
     * @param query 查询条件（含分页参数）
     * @return 分页费用明细数据
     */
    JsonVO<PageDTO<FeeDetailVO>> queryFeeDetail(FeeDetailQuery query);

    /**
     * 门诊费用明细报表导出
     * @param query 查询条件（与列表查询条件一致）
     * @return 导出文件下载URL
     */
    JsonVO<String> exportFeeDetail(FeeDetailQuery query);

    /**
     * 获取流量趋势
     * @param query 查询条件（日期范围、统计粒度）
     * @return 流量趋势数据点列表
     */
    JsonVO<List<TrafficTrendVO>> queryTrafficTrend(TrafficTrendQuery query);
}
