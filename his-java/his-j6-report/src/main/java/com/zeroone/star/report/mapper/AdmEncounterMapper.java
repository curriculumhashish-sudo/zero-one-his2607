package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.query.j6.report.FeeDetailExportQuery;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.report.entity.AdmEncounter;
import com.zeroone.star.report.entity.FeeDetailExportDO;
import com.zeroone.star.report.entity.FeeDetailLineDO;
import com.zeroone.star.report.entity.FeeDetailSummaryDO;
import com.zeroone.star.report.entity.PageEncounterDO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

public interface AdmEncounterMapper extends BaseMapper<AdmEncounter> {

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

    /** 全量导出（不加分页，含就诊层字段） */
    List<FeeDetailExportDO> selectAllExportLines(@Param("query") FeeDetailExportQuery query);
}
