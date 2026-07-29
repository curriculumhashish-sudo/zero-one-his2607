package com.zeroone.star.clinics.mapper;

import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;
import org.springframework.boot.actuate.endpoint.OperationType;

@Mapper
public interface ScheduleMapper {
    /**
     * 查询日程列表
     */
    //JsonVO<PageDTO<ScheduleDTO>> listQuery(ScheduleListQuery scheduleListQuery);


    /**
     * 获取日程详情
     * @param scheduleId 日程ID
     * @return 日程详情
     */
    @Select("select * from wkf_schedule where id = #{scheduleId}")
    ScheduleDTO getById(Long scheduleId);

    /**
     * 新增日程
     */
    void insertSchedule(Schedule schedule);



}
