package com.zeroone.star.calendar.controller;

import com.zeroone.star.project.dto.j8.calendar.SlotBlockDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotDetailDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotOverbookDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotPageResultDTO;
import com.zeroone.star.project.dto.j8.calendar.SlotUnblockDTO;
import com.zeroone.star.project.j8.calendar.SlotApis;
import com.zeroone.star.project.query.j8.calendar.SlotQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：号源日历控制器（桩代码，待后续实现业务逻辑）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j8/calendar")
@Tag(name = "号源日历")
public class SlotCalendarController implements SlotApis {

    @Operation(summary = "获取号源列表（分页 + 多条件查询 + 聚合统计）")
    @Override
    @GetMapping("/pageSlots")
    public JsonVO<SlotPageResultDTO> pageSlots(SlotQuery query) {
        return null;
    }

    @Operation(summary = "获取号源详情")
    @Override
    @GetMapping("/getSlotDetail")
    public JsonVO<SlotDetailDTO> getSlotDetail(String slotId) {
        return null;
    }

    @Operation(summary = "批量封锁号源")
    @Override
    @PutMapping("/blockSlots")
    public JsonVO<String> blockSlots(@RequestBody SlotBlockDTO dto) {
        return null;
    }

    @Operation(summary = "批量解封号源")
    @Override
    @PutMapping("/unblockSlots")
    public JsonVO<String> unblockSlots(@RequestBody SlotUnblockDTO dto) {
        return null;
    }

    @Operation(summary = "标记超售")
    @Override
    @PutMapping("/setOverbooked")
    public JsonVO<String> setOverbooked(@RequestBody SlotOverbookDTO dto) {
        return null;
    }
}
