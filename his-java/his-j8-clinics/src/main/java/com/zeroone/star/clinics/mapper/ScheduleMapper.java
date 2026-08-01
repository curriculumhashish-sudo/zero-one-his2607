package com.zeroone.star.clinics.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.entity.ScheduleActor;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

@Mapper
public interface ScheduleMapper extends BaseMapper<Schedule> {

    /**
     * 按条件查询日程列表（不含分页，用于日历视图聚合）
     */
    List<Schedule> selectByConditions(@Param("query") ScheduleListQuery query);

    /**
     * 根据日程ID列表批量查询参与者
     */
    List<ScheduleActor> selectActorsByScheduleIds(@Param("scheduleIds") List<Long> scheduleIds);

    /**
     * 获取日程详情
     */
    ScheduleDTO getById(Long scheduleId);

    /**
     * 新增日程
     */
    void insertSchedule(Schedule schedule);
}