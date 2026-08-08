package com.zeroone.star.statdata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.statdata.entity.WkfInventoryItem;
import org.apache.ibatis.annotations.Param;

import java.util.List;
import java.util.Map;

public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {

    List<Map<String, Object>> getExpireWarningData(@Param("itemTable") String itemTable);
}