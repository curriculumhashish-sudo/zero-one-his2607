package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.StockInOutVO;
import com.zeroone.star.statdata.entity.WkfSupplyRequest;
import com.zeroone.star.statdata.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.statdata.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.time.YearMonth;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * <p>
 * 库存出入库图表服务实现类
 * </p>
 *
 * @author highxin
 * @since 2026-07-24
 */
@Service
public class WkfSupplyRequestServiceImpl extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest> implements IWkfSupplyRequestService {

    private static final Map<String, String> TYPE_NAME_MAP = new HashMap<>();

    static {
        TYPE_NAME_MAP.put("med_medication_definition", "药品");
        TYPE_NAME_MAP.put("med_consumable_definition", "耗材");
        TYPE_NAME_MAP.put("adm_device_definition", "器械");
    }

    @Override
    public StockInOutVO getStockChartDTO(HourTrendQuery query) {
        String dateFrom = query.getDateFrom();
        String dateTo = query.getDateTo();

        LocalDate startDate;
        LocalDate endDate;

        if (dateFrom == null || dateFrom.isEmpty() || dateTo == null || dateTo.isEmpty()) {
            LocalDate now = LocalDate.now();
            int currentMonth = now.getMonthValue();
            int year = now.getYear();
            if (currentMonth <= 6) {
                startDate = LocalDate.of(year, 1, 1);
                endDate = LocalDate.of(year, 6, 30);
            } else {
                startDate = LocalDate.of(year, 7, 1);
                endDate = LocalDate.of(year, 12, 31);
            }
        } else {
            startDate = LocalDate.parse(dateFrom);
            endDate = LocalDate.parse(dateTo);
        }

        List<String> months = generateMonthList(startDate, endDate);

        String startDateStr = startDate.toString();
        String endDateStr = endDate.plusDays(1).toString();

        List<Map<String, Object>> inData = getBaseMapper().getStockInByMonth(startDateStr, endDateStr);
        List<Map<String, Object>> outData = getBaseMapper().getStockOutByMonth(startDateStr, endDateStr);

        return buildResult(months, inData, outData);
    }

    private List<String> generateMonthList(LocalDate start, LocalDate end) {
        List<String> list = new ArrayList<>();
        YearMonth ym = YearMonth.from(start);
        YearMonth endYm = YearMonth.from(end);
        while (!ym.isAfter(endYm)) {
            list.add(ym.format(DateTimeFormatter.ofPattern("yyyy-MM")));
            ym = ym.plusMonths(1);
        }
        return list;
    }

    private StockInOutVO buildResult(List<String> months, List<Map<String, Object>> inData, List<Map<String, Object>> outData) {
        StockInOutVO vo = new StockInOutVO();
        vo.setCategories(months);
        vo.setUnit("元");

        Map<String, Map<String, Double>> inMap = convertToMap(inData);
        Map<String, Map<String, Double>> outMap = convertToMap(outData);

        List<StockInOutVO.StockInOutSeriesItem> series = new ArrayList<>();

        for (String tableName : TYPE_NAME_MAP.keySet()) {
            String name = TYPE_NAME_MAP.get(tableName);

            StockInOutVO.StockInOutSeriesItem inItem = new StockInOutVO.StockInOutSeriesItem();
            inItem.setName(name);
            inItem.setDirection("入库");
            inItem.setValues(buildData(months, inMap.getOrDefault(tableName, new HashMap<>())));
            series.add(inItem);

            StockInOutVO.StockInOutSeriesItem outItem = new StockInOutVO.StockInOutSeriesItem();
            outItem.setName(name);
            outItem.setDirection("出库");
            outItem.setValues(buildData(months, outMap.getOrDefault(tableName, new HashMap<>())));
            series.add(outItem);
        }

        vo.setSeries(series);
        return vo;
    }

    private Map<String, Map<String, Double>> convertToMap(List<Map<String, Object>> data) {
        Map<String, Map<String, Double>> result = new HashMap<>();
        for (Map<String, Object> row : data) {
            String type = (String) row.get("material_type");
            String month = (String) row.get("month");
            Double amount = row.get("total_amount") != null ? ((Number) row.get("total_amount")).doubleValue() : 0.0;
            result.computeIfAbsent(type, k -> new HashMap<>()).put(month, amount);
        }
        return result;
    }

    private List<Double> buildData(List<String> months, Map<String, Double> data) {
        List<Double> result = new ArrayList<>();
        for (String month : months) {
            result.add(data.getOrDefault(month, 0.0));
        }
        return result;
    }
}