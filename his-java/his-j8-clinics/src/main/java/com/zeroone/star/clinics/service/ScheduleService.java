package com.zeroone.star.clinics.service;

import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.j8.clinics.ScheduleCreateVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleItemVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleListVO;

public interface ScheduleService {
    /**
     * 查询日程列表
     * @param scheduleListQuery
     * @return
     */
    ScheduleListVO listQuery(ScheduleListQuery scheduleListQuery);

    /**
     * 根据ID查询日程
     * @param scheduleId
     * @return
     */
    ScheduleItemVO getById(String scheduleId);

    /**
     * 新增日程
     */
    ScheduleCreateVO insertSchedule(ScheduleAddDTO dto);

    /**
     * 生成正式排班
     * @param formalRosterDTO 生成正式排班参数
     * @return 操作结果
     */
    String generateFormalRoster(com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO formalRosterDTO);
}