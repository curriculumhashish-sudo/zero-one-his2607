package com.zeroone.star.project.j8.clinics;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：门诊日程接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author www
 * @version 1.0.0
 */
public interface ClinicsApis {
    /**
     * 获取日程列表
     * @param scheduleListQuery 查询门诊日程参数
     * @return 门诊日程分页结果
     */
    JsonVO<PageDTO<ScheduleDTO>> querySchedule(ScheduleListQuery scheduleListQuery);

    /**
     * 获取日程详情
     * @param scheduleId 日程ID
     * @return 日程详情
     */
    JsonVO<ScheduleDTO> queryScheduleDetail(Long scheduleId);

    /**
     * 新建日程
     * @param scheduleAddDTO 新建日程参数
     * @return 新建日程结果
     */
    JsonVO<String> createSchedule(ScheduleAddDTO scheduleAddDTO);

    /**
     * 生成正式排班
     * @return 生成正式排班结果
     */
    JsonVO<String> generateFormalRoster(FormalRosterDTO formalRosterDTO);




}
