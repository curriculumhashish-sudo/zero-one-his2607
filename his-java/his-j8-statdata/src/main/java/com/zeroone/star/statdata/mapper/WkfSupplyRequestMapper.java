package com.zeroone.star.statdata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.statdata.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;

import java.util.List;
import java.util.Map;

public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    List<Map<String, Object>> getStockInByMonth(
            @Param("startDate") String startDate,
            @Param("endDate") String endDate);

    List<Map<String, Object>> getStockOutByMonth(
            @Param("startDate") String startDate,
            @Param("endDate") String endDate);
}