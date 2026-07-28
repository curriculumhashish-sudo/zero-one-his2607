package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.report.entity.WhaleAuditLogs;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * 描述：安全审计日志 Mapper 数据操作层
 */
@Mapper
public interface WhaleAuditLogsMapper extends BaseMapper<WhaleAuditLogs> {

    /**
     * 根据年份统计 1-12 各月份的访问量
     *
     * @param year 查询年
     * @return 各月份访问量数据点列表
     */
    @Select("SELECT " +
            "MONTH(created_at) AS `month`, " +
            "COUNT(*) AS `count` " +
            "FROM whale_security_logs " +
            "WHERE YEAR(created_at) = #{year} " +
            "GROUP BY MONTH(created_at) " +
            "ORDER BY `month` ASC")

    List<MonthlyAccessVO> selectMonthlyAccessCount(@Param("year") Integer year);
}
