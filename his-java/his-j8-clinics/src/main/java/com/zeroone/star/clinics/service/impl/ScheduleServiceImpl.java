package com.zeroone.star.clinics.service.impl;

import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.mapper.ScheduleMapper;
import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;


@Service
public class ScheduleServiceImpl implements ScheduleService {

    @Autowired
    private ScheduleMapper scheduleMapper;

    /**
     * 查询日程列表
     * @param scheduleListQuery
     * @return
     */
    @Override
    public JsonVO<PageDTO<ScheduleDTO>> listQuery(ScheduleListQuery scheduleListQuery) {
        return null;
    }

    /**
     * 根据ID查询日程
     * @param scheduleId
     * @return
     */
    @Override
    public ScheduleDTO getById(Long scheduleId) {
        return scheduleMapper.getById(scheduleId);
    }

    /**
     * 新增日程
     * @param dto
     */
    @Override
    public void insertSchedule(ScheduleAddDTO dto) {
        Schedule schedule = new Schedule();
        BeanUtils.copyProperties(dto,schedule);
        scheduleMapper.insertSchedule(schedule);

    }
}
