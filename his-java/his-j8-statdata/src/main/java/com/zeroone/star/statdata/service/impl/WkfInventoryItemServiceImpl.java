package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.dto.j8.statdata.ExpireWarningDTO;
import com.zeroone.star.statdata.entity.WkfInventoryItem;
import com.zeroone.star.statdata.mapper.WkfInventoryItemMapper;
import com.zeroone.star.statdata.service.IWkfInventoryItemService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
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
    public List<ExpireWarningDTO> getExpireWarningData(String goodsType) {
        String itemTable = null;
        if (goodsType != null && !goodsType.isEmpty() && !"all".equalsIgnoreCase(goodsType)) {
            itemTable = GOODS_TYPE_MAP.get(goodsType.toLowerCase());
        }
        
        List<Map<String, Object>> data = getBaseMapper().getExpireWarningData(itemTable);
        
        List<ExpireWarningDTO> result = new ArrayList<>();
        Map<String, Integer> dataMap = new HashMap<>();
        dataMap.put("expireInOneMonth", 0);
        dataMap.put("expireInThreeMonth", 0);
        dataMap.put("safe", 0);
        
        for (Map<String, Object> row : data) {
            String name = (String) row.get("name");
            Integer value = row.get("value") != null ? ((Number) row.get("value")).intValue() : 0;
            dataMap.put(name, value);
        }
        
        for (String key : dataMap.keySet()) {
            ExpireWarningDTO dto = new ExpireWarningDTO();
            dto.setName(key);
            dto.setValue(dataMap.get(key));
            result.add(dto);
        }
        
        return result;
    }
}