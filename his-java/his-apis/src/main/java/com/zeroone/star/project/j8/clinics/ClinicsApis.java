package com.zeroone.star.project.j8.clinics;

import com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleItemVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleListVO;

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
     * 查询日程列表
     * @param scheduleListQuery 查询门诊日程参数
     * @return 日程列表响应
     */
    JsonVO<ScheduleListVO> querySchedule(ScheduleListQuery scheduleListQuery);


    /**
     * 获取日程详情
     * @param scheduleId 日程ID
     * @return 日程详情
     */
    JsonVO<ScheduleItemVO> queryScheduleDetail(String scheduleId);

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