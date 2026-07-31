package com.zeroone.star.report.service.impl;

import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import com.zeroone.star.report.mapper.WhaleAuditLogsMapper;
import com.zeroone.star.report.mapper.WkfInventoryItemMapper;
import com.zeroone.star.report.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.report.service.IReportService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：报表中心 业务逻辑实现类
 * </p>
 * @author j6
 * @version 1.0.0
 */
@Service
public class ReportServiceImpl implements IReportService {

    @Resource
    private WhaleAuditLogsMapper whaleAuditLogsMapper;

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    @Override
    public List<MonthlyAccessVO> getMonthlyAccess(MonthlyAccessQuery query) {
        // 1. 获取入参年份，若未传参则默认使用 2026 年
        Integer year = (query != null && query.getYear() != null) ? query.getYear() : 2026;

        // 2. 调用 Mapper 执行 SQL，查询数据库中存在的访问量统计
        List<MonthlyAccessVO> dbList = whaleAuditLogsMapper.selectMonthlyAccessCount(year);

        // 3. 将数据库查询出的结果转为 Map<月份, 访问次数> 结构
        Map<Integer, Long> countMap = (dbList != null) ?
                dbList.stream().collect(Collectors.toMap(MonthlyAccessVO::getMonth, MonthlyAccessVO::getCount, (v1, v2) -> v1)) :
                Collections.emptyMap();

        // 4. 循环 1 到 12 月，进行“数据补零”填充
        List<MonthlyAccessVO> resultList = new ArrayList<>();
        for (int month = 1; month <= 12; month++) {
            // 若数据库中有该月记录，取其真实 count；无记录则补 0L
            Long count = countMap.getOrDefault(month, 0L);
            resultList.add(new MonthlyAccessVO(month, count));
        }

        // 5. 返回 12 个月份完整的统计列表
        return resultList;
    }

    @Override
    public List<SupplyAmountCompareVO> getSupplyAmountCompare(SupplyAmountCompareQuery query) {
        // 1. 参数默认值防空处理
        String start = (query != null && query.getStart() != null) ? query.getStart() : "2026-01-01";
        String end = (query != null && query.getEnd() != null) ? query.getEnd() : "2026-12-31";

        // 2. 调用 Mapper 查数据库中物资出入库的统计金额
        List<SupplyAmountCompareVO> list = wkfSupplyRequestMapper.selectSupplyAmountCompare(start, end);

        // 3. 若无数据返回空集合
        return (list != null) ? list : Collections.emptyList();
    }

    @Override
    public List<ExpirationWarningVO> getExpirationWarning(ExpirationWarningQuery query) {
        // 1. 获取预警天数参数，若为空默认查询 90 天内预警记录
        Integer expireWithinDays = (query != null && query.getExpireWithinDays() != null) ? query.getExpireWithinDays() : 90;

        // 2. 调用 Mapper 执行有效期计算 SQL 查库
        List<ExpirationWarningVO> list = wkfInventoryItemMapper.selectExpirationWarningList(expireWithinDays);

        // 3. 防空处理
        return (list != null) ? list : Collections.emptyList();
    }
}


