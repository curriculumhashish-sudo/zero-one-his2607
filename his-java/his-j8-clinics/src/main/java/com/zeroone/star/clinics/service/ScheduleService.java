package com.zeroone.star.clinics.service;

import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface ScheduleService {
    /**
     * 查询日程列表
     * @param scheduleListQuery
     * @return
     */
    JsonVO<PageDTO<ScheduleDTO>> listQuery(ScheduleListQuery scheduleListQuery);

    /**
     * 根据ID查询日程
     * @param scheduleId
     * @return
     */
    ScheduleDTO getById(Long scheduleId);

    /**
     * 新增日程
     */
    void insertSchedule(ScheduleAddDTO dto);

    /**
     * 生成正式排班
     * @param formalRosterDTO 生成正式排班参数
     * @return 操作结果
     */
    String generateFormalRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO);
}