package com.zeroone.star.setroster.service;

import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.j8.setroster.DailySchedulePageVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleVO;
import com.zeroone.star.setroster.entity.BkgScheduleTemplate;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * T2 排班日模板（07 §5.3） 服务类
 * </p>
 *
 * @author 墨
 * @since 2026-07-21
 */
public interface IBkgScheduleTemplateService extends IService<BkgScheduleTemplate> {

    /**
     * 查询排班日模板选项列表
     * @return 排班日模板选项列表
     */
    DailySchedulePageVO<DailyScheduleVO> listDailyScheduleOptions(DailyScheduleQuery query);

    /**
     * 分页查询排班日模板
     * @param query 查询条件
     * @return 排班日模板分页结果
     */
    DailySchedulePageVO<DailyScheduleVO> pageDailySchedule(DailyScheduleQuery query);

    /**
     * 新增排班日模板
     * @param dto 新增参数
     * @return 新增结果
     */
    String addDailySchedule(AddDailyScheduleDTO dto);

    /**
     * 修改排班日模板
     * @param dto 修改参数
     * @return 修改结果
     */
    String modifyDailySchedule(DailyScheduleDTO dto);

    /**
     * 删除排班日模板
     * @param id 删除参数
     * @return 删除结果
     */
    String deleteDailySchedule(String id);

}
