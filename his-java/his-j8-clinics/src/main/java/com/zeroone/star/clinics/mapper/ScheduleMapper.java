package com.zeroone.star.clinics.mapper;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Constants;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.springframework.boot.actuate.endpoint.OperationType;

@Mapper
public interface ScheduleMapper extends BaseMapper<Schedule> {
    /**
     * 分页查询日程信息
     */
    //Page<Schedule> selectPage(Page<Schedule> page, @Param(Constants.WRAPPER) Wrapper<Schedule> wrapper);


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