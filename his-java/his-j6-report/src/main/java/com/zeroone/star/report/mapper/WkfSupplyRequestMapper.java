package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import com.zeroone.star.report.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * 描述：物资出入库申请 Mapper 数据操作层
 */
@Mapper
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    /**
     * 按日期范围分组统计各月份入库与出库的总金额
     *
     * @param start 开始时间 (如 2026-01-01)
     * @param end   结束时间 (如 2026-06-30)
     * @return 各月份出入库金额对比数据点列表
     */
    @Select("SELECT " +
            "CASE WHEN type_enum LIKE '%IN%' THEN 'INBOUND' ELSE 'OUTBOUND' END AS `type`, " +
            "DATE_FORMAT(created_at, '%Y-%m') AS `period`, " +
            "SUM(total_price) AS `totalAmount` " +
            "FROM wkf_supply_request " +
            "WHERE is_deleted = 0 " +
            "AND created_at >= #{start} " +
            "AND created_at <= #{end} " +
            "GROUP BY `type`, `period` " +
            "ORDER BY `period` ASC")
    List<SupplyAmountCompareVO> selectSupplyAmountCompare(@Param("start") String start, @Param("end") String end);
}
