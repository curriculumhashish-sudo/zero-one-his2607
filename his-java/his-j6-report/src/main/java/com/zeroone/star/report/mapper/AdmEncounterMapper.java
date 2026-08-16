package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.query.j6.report.FeeDetailExportQuery;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.report.entity.AdmEncounter;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import com.zeroone.star.report.entity.FeeDetailExportDO;
import com.zeroone.star.report.entity.FeeDetailLineDO;
import com.zeroone.star.report.entity.FeeDetailSummaryDO;
import com.zeroone.star.report.entity.PageEncounterDO;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDate;
import java.util.List;
import java.util.Map;

/**
 * 接诊量统计 Mapper 接口
 * @author j6小组-TKORHP
 */
public interface AdmEncounterMapper extends BaseMapper<AdmEncounter> {

    /**
     * 按日期范围统计每日接诊量
     * @param startDate 开始日期
     * @param endDate   结束日期
     * @return 每日接诊量列表，每项包含 date(日期) 和 value(接诊人数)
     */
    List<Map<String, Object>> selectDailyEncounterCount(@Param("startDate") LocalDate startDate,
                                                        @Param("endDate") LocalDate endDate);
    /** 统计符合条件的就诊数 */
    long selectEncounterCount(@Param("query") FeeDetailQuery query);

    /** 手动分页查询就诊列表 */
    List<PageEncounterDO> selectEncounterByPage(@Param("query") FeeDetailQuery query,
                                                @Param("offset") long offset,
                                                @Param("limit") long limit);

    /** 根据就诊ID列表查询所有费用行 */
    List<FeeDetailLineDO> selectFeeDetailLineByEncounterIds(
            @Param("encounters") List<String> encounterIds,
            @Param("query") FeeDetailQuery query);

    /** 门诊费用汇总（与分页查询相同筛选条件，全量统计） */
    FeeDetailSummaryDO selectFeeDetailSummary(@Param("query") FeeDetailQuery query);

    /**
     * 统计总就诊人数
     * @return 未删除的就诊记录总数
     */
    int selectTotalEncounterCount();
    /** 全量导出（不加分页，含就诊层字段） */
    List<FeeDetailExportDO> selectAllExportLines(@Param("query") FeeDetailExportQuery query);

    /** 门诊流量趋势（按时间粒度统计门诊数量） */
    List<TrafficTrendVO> selectTrafficTrend(@Param("start") String start,
                                            @Param("end") String end,
                                            @Param("granularity") String granularity);

    /** 门诊流量趋势原始数据（返回 Map 供 Service 层转换） */
    List<Map<String, Object>> selectTrafficTrendRaw(@Param("start") String start,
                                                     @Param("end") String end,
                                                     @Param("granularity") String granularity);
}
