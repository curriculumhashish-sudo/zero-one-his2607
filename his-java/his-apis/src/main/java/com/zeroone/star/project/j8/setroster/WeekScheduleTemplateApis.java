package com.zeroone.star.project.j8.setroster;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.query.j8.setroster.WeekScheduleTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;

/**
 * <p>
 * 描述：周排班模板接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
public interface WeekScheduleTemplateApis {

    /**
     * 分页查询周排班模板列表
     * @param query 查询条件
     * @return 周排班模板列表
     */
    JsonVO<PageDTO<WeekScheduleTemplateDTO>> pageWeekScheduleTemplate(WeekScheduleTemplateQuery query);

    /**
     * 查询周排班模板详情
     * @param id 周排班模板ID
     * @return 周排班模板详情
     */
    JsonVO<WeekScheduleTemplateDetailVO> getWeekScheduleTemplate(Long id);

    /**
     * 新增周排班模板
     * @param dto 新增周排班模板传输对象
     * @return 新增周排班模板ID
     */
    JsonVO<String> addWeekScheduleTemplate(AddWeekScheduleTemplateDTO dto);

    /**
     * 修改周排班模板
     * @param dto 修改周排班模板传输对象
     * @return 修改周排班模板ID
     */
    JsonVO<String> modifyWeekScheduleTemplate(WeekScheduleTemplateDTO dto);

    /**
     * 删除周排班模板
     * @param id 周排班模板ID
     * @return 删除周排班模板ID
     */
    JsonVO<String> deleteWeekScheduleTemplate(Long id);
}

