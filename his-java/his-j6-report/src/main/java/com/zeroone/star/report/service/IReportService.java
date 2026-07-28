package com.zeroone.star.report.service;

import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;

import java.util.List;

/**
 * <p>
 * 描述：报表中心 业务逻辑接口
 * </p>
 * @author j6
 * @version 1.0.0
 */
public interface IReportService {

    /**
     * 获取月访问量（含 1-12 月数据补零逻辑）
     *
     * @param query 查询参数（包含年份 year）
     * @return 1~12 月完整的访问量趋势列表
     */
    List<MonthlyAccessVO> getMonthlyAccess(MonthlyAccessQuery query);

    /**
     * 物资入库与出库金额对比
     *
     * @param query 查询参数（包含开始时间 start 与结束时间 end）
     * @return 各月份出入库金额对比数据点列表
     */
    List<SupplyAmountCompareVO> getSupplyAmountCompare(SupplyAmountCompareQuery query);

    /**
     * 库存物资有效期预警分布
     *
     * @param query 查询参数（包含预警天数 expireWithinDays）
     * @return 预警物资数据点列表
     */
    List<ExpirationWarningVO> getExpirationWarning(ExpirationWarningQuery query);
}


