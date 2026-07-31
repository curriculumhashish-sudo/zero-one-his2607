package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.report.entity.AdmEncounter;
import com.zeroone.star.report.entity.FeeDetailLineDO;
import com.zeroone.star.report.entity.FeeDetailSummaryDO;
import com.zeroone.star.report.entity.PageEncounterDO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

public interface AdmEncounterMapper extends BaseMapper<AdmEncounter> {
    IPage<PageEncounterDO> selectEncounterByPage(
            IPage<PageEncounterDO> page,
            @Param(("query")) FeeDetailQuery query);

    List<FeeDetailLineDO> selectFeeDetailLineByEncounterIds(
            @Param("encounters") List<String> encounterIds,
            @Param("query") FeeDetailQuery query);

    /** 门诊费用汇总（与分页查询相同筛选条件，全量统计） */
    FeeDetailSummaryDO selectFeeDetailSummary(@Param("query") FeeDetailQuery query);
}
