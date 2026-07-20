package com.zeroone.star.project.j8.setroster;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleOptionVO;

import java.util.List;

/**
 * 描述：日排班模板接口
 */
public interface DailyScheduleApis {

    /**
     * 获取日排班模板下拉选项（用于周排班选择）
     * @return 日排班模板下拉选项列表
     */
    JsonVO<List<DailyScheduleOptionVO>> listDailyScheduleOptions();

    /**
     * 获取日排班模板列表(条件+分页)
     * @param query 查询条件
     * @return 日排班模板列表
     */
    JsonVO<PageDTO<DailyScheduleDTO>> pageDailySchedule(DailyScheduleQuery query);

    /**
     * 新增日排班模板
     * @param dto 新增日排班模板传输对象
     * @return 新增日排班模板ID
     */
    JsonVO<String> addDailySchedule(AddDailyScheduleDTO dto);

    /**
     * 编辑日排班模板
     * @param dto 编辑日排班模板传输对象
     * @return 编辑日排班模板ID
     */
    JsonVO<String> modifyDailySchedule(DailyScheduleDTO dto);

    /**
     * 删除日排班模板
     * @param id 日排班模板ID
     * @return 删除日排班模板ID
     */
    JsonVO<String> deleteDailySchedule(String id);
}
