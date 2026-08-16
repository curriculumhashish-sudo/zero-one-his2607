package com.zeroone.star.setroster.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.query.j8.setroster.WeekScheduleTemplateQuery;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;
import com.zeroone.star.setroster.entity.WeekScheduleTemplate;

/**
 * <p>
 * 描述：周排班模板 Service 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
public interface WeekScheduleTemplateService extends IService<WeekScheduleTemplate> {

    /**
     * 分页查询周排班模板列表
     * @param query 查询条件
     * @return 分页结果
     */
    PageDTO<WeekScheduleTemplateDTO> page(WeekScheduleTemplateQuery query);

    /**
     * 查询周排班模板详情
     * @param id 模板ID
     * @return 模板详情
     */
    WeekScheduleTemplateDetailVO getDetail(String id);

    /**
     * 新增周排班模板
     * @param dto 新增传输对象
     * @return 模板ID
     */
    String add(AddWeekScheduleTemplateDTO dto);

    /**
     * 修改周排班模板
     * @param dto 修改传输对象
     * @return 模板ID
     */
    String edit(WeekScheduleTemplateDTO dto);

    /**
     * 删除周排班模板
     * @param id 模板ID
     * @return 是否删除成功
     */
    boolean delete(String id);
}
