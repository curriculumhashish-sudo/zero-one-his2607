package com.zeroone.star.clinics.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.entity.ScheduleActor;
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
    List<ScheduleActor> selectActorsByScheduleIds(@Param("scheduleIds") List<String> scheduleIds);

    /**
     * 查询单个日程的参与者
     */
    List<ScheduleActor> selectActorsByScheduleId(@Param("scheduleId") String scheduleId);

    /**
     * 获取日程详情
     */
    Schedule getById(@Param("scheduleId") String scheduleId);

    /**
     * 查询医师名称
     */
    String selectPractitionerName(@Param("id") String id);

    /**
     * 查询地点名称
     */
    String selectLocationName(@Param("id") String id);

    /**
     * 查询医疗服务名称
     */
    String selectHealthcareServiceName(@Param("id") String id);

    /**
     * 新增日程
     */
    void insertSchedule(Schedule schedule);
}
