package com.zeroone.star.calendar.controller;

import com.zeroone.star.calendar.service.ISlotCalendarService;
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
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：号源日历管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@RestController
@Tag(name = "号源日历管理")
public class SlotCalendarController implements SlotApis {

    @Resource
    private ISlotCalendarService slotCalendarService;

    @Operation(summary = "获取号源列表（分页 + 聚合统计）")
    @GetMapping("/pageSlots")
    @Override
    public JsonVO<SlotPageResultDTO> pageSlots(SlotQuery query) {
        return slotCalendarService.pageSlots(query);
    }

    @Operation(summary = "获取号源详情")
    @GetMapping("/getSlotDetail")
    @Override
    public JsonVO<SlotDetailDTO> getSlotDetail(String slotId) {
        return slotCalendarService.getSlotDetail(slotId);
    }

    @Operation(summary = "批量封锁号源（停诊）")
    @PutMapping("/blockSlots")
    @Override
    public JsonVO<String> blockSlots(@RequestBody SlotBlockDTO dto) {
        return slotCalendarService.blockSlots(
                dto.getSlotIds().toArray(new String[0]),
                dto.getReason());
    }

    @Operation(summary = "批量解封号源（恢复可约）")
    @PutMapping("/unblockSlots")
    @Override
    public JsonVO<String> unblockSlots(@RequestBody SlotUnblockDTO dto) {
        return slotCalendarService.unblockSlots(
                dto.getSlotIds().toArray(new String[0]));
    }

    @Operation(summary = "标记超售")
    @PutMapping("/setOverbooked")
    @Override
    public JsonVO<String> setOverbooked(@RequestBody SlotOverbookDTO dto) {
        return slotCalendarService.setOverbooked(dto.getSlotId(), dto.getOverbooked());
    }
}
