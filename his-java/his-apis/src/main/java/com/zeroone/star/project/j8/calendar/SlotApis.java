package com.zeroone.star.project.j8.calendar;

import com.zeroone.star.project.dto.j8.calendar.SlotBlockDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDetailDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotOverbookDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotPageResultDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotUnblockDTO;
import com.zeroone.star.project.query.j8.calendar.SlotQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：号源日历管理接口（号源日历列表页）
 * </p>
 *
 * <h3>接口列表</h3>
 * <table>
 *   <tr><th>方法</th><th>HTTP</th><th>路径</th><th>说明</th></tr>
 *   <tr><td>pageSlots</td><td>GET</td><td>/calendar/pageSlots</td><td>获取号源列表（分页 + 多条件查询 + 聚合统计）</td></tr>
 *   <tr><td>getSlotDetail</td><td>GET</td><td>/calendar/getSlotDetail</td><td>获取号源详情</td></tr>
 *   <tr><td>blockSlots</td><td>PUT</td><td>/calendar/blockSlots</td><td>批量封锁号源</td></tr>
 *   <tr><td>unblockSlots</td><td>PUT</td><td>/calendar/unblockSlots</td><td>批量解封号源</td></tr>
 *   <tr><td>setOverbooked</td><td>PUT</td><td>/calendar/setOverbooked</td><td>标记超售</td></tr>
 * </table>
 *
 * <h3>号源状态说明</h3>
 * <ul>
 *   <li><b>free</b> — 可约</li>
 *   <li><b>busy</b> — 已满</li>
 *   <li><b>busy-unavailable</b> — 停诊</li>
 * </ul>
 *
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
public interface SlotApis {

    /**
     * 获取号源列表（分页 + 多条件查询 + 聚合统计）
     *
     * <p>顶部筛选条件：科室、日期（必填）、医师、号源状态</p>
     * <p>返回聚合统计用于顶部文案：
     * <code>2026-07-19 周日 本页N个时段 容量X 已约Y 剩余Z</code></p>
     *
     * @param query 分页查询条件
     * @return 分页结果（含聚合统计 + 号源列表）
     */
    JsonVO<SlotPageResultDTO> pageSlots(SlotQuery query);

    /**
     * 获取号源详情（详情弹窗/抽屉）
     *
     * @param slotId 号源主键
     * @return 号源详情（比列表多备注等字段）
     */
    JsonVO<SlotDetailDTO> getSlotDetail(String slotId);

    /**
     * 批量封锁号源（将选中号源状态置为 busy-unavailable）
     *
     * @param dto 封锁请求（号源ID列表 + 封锁原因）
     * @return 操作结果（通过 code 判断）
     */
    JsonVO<Void> blockSlots(SlotBlockDTO dto);

    /**
     * 批量解封号源（将已封锁的号源恢复为可约状态）
     *
     * @param dto 解封请求（号源ID列表）
     * @return 操作结果（通过 code 判断）
     */
    JsonVO<Void> unblockSlots(SlotUnblockDTO dto);

    /**
     * 标记超售（设置号源的超售开关）
     *
     * @param dto 超售请求（号源ID + 超售标记）
     * @return 操作结果（通过 code 判断）
     */
    JsonVO<Void> setOverbooked(SlotOverbookDTO dto);
}
