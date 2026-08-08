package com.zeroone.star.project.j8.setroster;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionAddDto;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionUpdateDto;
import com.zeroone.star.project.query.j8.setroster.CalendarExceptionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.CalendarExceptionVo;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

/**
 * <p>描述: 排班设置-节日例外 API 接口定义</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Tag(name = "节日例外管理", description = "用于管理日历中的特殊日期配置（如节假日、调休等）")
public interface CalendarExceptionApi {

    /**
     * 获取节日例外列表（条件筛选）
     *
     * @param query 查询条件（机构、日期范围、类型、关键字）
     * @return 节日例外列表
     */
    @Operation(summary = "获取节日例外列表", description = "根据条件筛选系统中配置的节日例外数据")
    JsonVO<PageDTO<CalendarExceptionVo>> queryCalendarExceptions(CalendarExceptionQuery query);

    /**
     * 新建节日例外
     *
     * @param data 新增数据传输对象，包含类型、日期范围等信息
     * @return 操作结果消息
     */
    @Operation(summary = "新建节日例外", description = "创建一个新的节日例外配置")
    JsonVO<String> add(CalendarExceptionAddDto data);

    /**
     * 修改节日例外
     *
     * @param data 更新数据传输对象，必须包含主键ID
     * @return 操作结果消息
     */
    @Operation(summary = "修改节日例外", description = "根据ID更新现有的节日例外配置")
    JsonVO<String> modify(CalendarExceptionUpdateDto data);

    /**
     * 删除节日例外
     *
     * @param id 节日例外的主键ID
     * @return 操作结果消息
     */
    @Operation(summary = "删除节日例外", description = "根据ID物理删除或逻辑删除指定的节日例外")
    JsonVO<String> delete(@Parameter(description = "节日例外ID", required = true) String id);
}
