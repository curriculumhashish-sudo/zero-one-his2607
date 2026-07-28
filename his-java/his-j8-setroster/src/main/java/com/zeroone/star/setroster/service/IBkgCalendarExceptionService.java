package com.zeroone.star.setroster.service;

import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionAddDto;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionUpdateDto;
import com.zeroone.star.project.vo.j8.setroster.CalendarExceptionVo;

import java.util.List;

/**
 * <p>
 * T3 日历例外（07 §5.4）业务服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
public interface IBkgCalendarExceptionService {

    /**
     * 查询所有节日例外
     *
     * @return 节日例外 VO 列表
     */
    List<CalendarExceptionVo> queryAll();

    /**
     * 新增节日例外
     *
     * @param dto 新增 DTO
     * @return 新增记录的主键 ID
     */
    String add(CalendarExceptionAddDto dto);

    /**
     * 修改节日例外
     *
     * @param dto 修改 DTO（必须含 id）
     * @return 修改记录的主键 ID
     */
    String modify(CalendarExceptionUpdateDto dto);

    /**
     * 逻辑删除节日例外
     *
     * @param id 主键 ID
     * @return 被删除记录的主键 ID
     */
    String delete(String id);
}
