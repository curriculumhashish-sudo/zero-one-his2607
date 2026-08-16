package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.ExpireWarningVO;
import com.zeroone.star.statdata.entity.WkfInventoryItem;
import com.zeroone.star.statdata.mapper.WkfInventoryItemMapper;
import com.zeroone.star.statdata.service.IWkfInventoryItemService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * <p>
 * 库存物品服务实现类
 * </p>
 *
 * @author highxin
 * @since 2026-07-24
 */
@Service
public class WkfInventoryItemServiceImpl extends ServiceImpl<WkfInventoryItemMapper, WkfInventoryItem> implements IWkfInventoryItemService {

    private static final Map<String, String> GOODS_TYPE_MAP = new HashMap<>();

    static {
        GOODS_TYPE_MAP.put("drug", "med_medication_definition");
        GOODS_TYPE_MAP.put("consumable", "med_consumable_definition");
        GOODS_TYPE_MAP.put("device", "adm_device_definition");
    }

    @Override
    public ExpireWarningVO getExpireWarningData(HourTrendQuery query) {
        String goodsType = query.getGranularity();

        String itemTable = null;
        if (goodsType != null && !goodsType.isEmpty() && !"all".equalsIgnoreCase(goodsType)) {
            itemTable = GOODS_TYPE_MAP.get(goodsType.toLowerCase());
        }

        List<Map<String, Object>> data = getBaseMapper().getExpireWarningData(itemTable);

        Map<String, Integer> dataMap = new HashMap<>();
        dataMap.put("expireInOneMonth", 0);
        dataMap.put("expireInThreeMonth", 0);
        dataMap.put("safe", 0);

        for (Map<String, Object> row : data) {
            String name = (String) row.get("name");
            Integer value = row.get("value") != null ? ((Number) row.get("value")).intValue() : 0;
            dataMap.put(name, value);
        }

        List<ExpireWarningVO.ExpireWarningItem> items = new ArrayList<>();
        for (String key : dataMap.keySet()) {
            ExpireWarningVO.ExpireWarningItem item = new ExpireWarningVO.ExpireWarningItem();
            item.setName(key);
            item.setValue(dataMap.get(key));
            items.add(item);
        }

        ExpireWarningVO vo = new ExpireWarningVO();
        vo.setItems(items);
        vo.setUnit("种");
        return vo;
    }
}