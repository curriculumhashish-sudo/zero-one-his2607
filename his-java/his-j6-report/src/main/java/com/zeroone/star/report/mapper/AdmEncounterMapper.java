package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.report.entity.AdmEncounter;
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

    /**
     * 统计总就诊人数
     * @return 未删除的就诊记录总数
     */
    int selectTotalEncounterCount();
}
