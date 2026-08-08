package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.dto.j8.statdata.StockChartDTO;
import com.zeroone.star.project.query.j8.statdata.StockChartQuery;
import com.zeroone.star.statdata.entity.WkfSupplyRequest;
import com.zeroone.star.statdata.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.statdata.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
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
    public StockChartDTO getStockChartDTO(StockChartQuery query) {
        String startMonth = query.getStartMonth();
        String endMonth = query.getEndMonth();

        if (startMonth == null || startMonth.isEmpty() || endMonth == null || endMonth.isEmpty()) {
            int currentMonth = LocalDate.now().getMonthValue();
            int year = LocalDate.now().getYear();
            if (currentMonth <= 6) {
                startMonth = year + "-01";
                endMonth = year + "-06";
            } else {
                startMonth = year + "-07";
                endMonth = year + "-12";
            }
        }

        List<String> months = generateMonthList(startMonth, endMonth);
        
        List<Map<String, Object>> inData = getBaseMapper().getStockInByMonth(startMonth, endMonth);
        List<Map<String, Object>> outData = getBaseMapper().getStockOutByMonth(startMonth, endMonth);

        return buildResult(months, inData, outData);
    }

    /**
     * 生成月份列表
     * @param start 开始月份
     * @param end   结束月份
     * @return 月份列表
     */
    private List<String> generateMonthList(String start, String end) {
        List<String> list = new ArrayList<>();
        int startYear = Integer.parseInt(start.substring(0, 4));
        int startMonth = Integer.parseInt(start.substring(5));
        int endYear = Integer.parseInt(end.substring(0, 4));
        int endMonth = Integer.parseInt(end.substring(5));

        int y = startYear;
        int m = startMonth;
        while (y < endYear || (y == endYear && m <= endMonth)) {
            list.add(String.format("%04d-%02d", y, m));
            m++;
            if (m > 12) {
                m = 1;
                y++;
            }
        }
        return list;
    }

    /**
     * 构建库存出入库图表数据
     * @param months 月份列表
     * @param inData  入库数据
     * @param outData 出库数据
     * @return 库存图表数据
     */
    private StockChartDTO buildResult(List<String> months, List<Map<String, Object>> inData, List<Map<String, Object>> outData) {
        StockChartDTO dto = new StockChartDTO();
        dto.setXAxis(months);

        Map<String, Map<String, Double>> inMap = convertToMap(inData);
        Map<String, Map<String, Double>> outMap = convertToMap(outData);

        List<StockChartDTO.SeriesItem> series = new ArrayList<>();

        for (String tableName : TYPE_NAME_MAP.keySet()) {
            String name = TYPE_NAME_MAP.get(tableName);
            
            StockChartDTO.SeriesItem inItem = new StockChartDTO.SeriesItem();
            inItem.setName(name + "入库");
            inItem.setStack("入库");
            inItem.setData(buildData(months, inMap.getOrDefault(tableName, new HashMap<>())));
            series.add(inItem);

            StockChartDTO.SeriesItem outItem = new StockChartDTO.SeriesItem();
            outItem.setName(name + "出库");
            outItem.setStack("出库");
            outItem.setData(buildData(months, outMap.getOrDefault(tableName, new HashMap<>())));
            series.add(outItem);
        }

        dto.setSeries(series);
        return dto;
    }

    /**
     * 转换数据为映射
     * @param data 原始数据
     * @return 映射数据
     */
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

    /**
     * 构建图表数据
     * @param months 月份列表
     * @param data   数据映射
     * @return 图表数据
     */
    private List<Double> buildData(List<String> months, Map<String, Double> data) {
        List<Double> result = new ArrayList<>();
        for (String month : months) {
            result.add(data.getOrDefault(month, 0.0));
        }
        return result;
    }
}