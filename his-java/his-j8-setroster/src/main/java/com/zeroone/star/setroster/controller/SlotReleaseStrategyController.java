package com.zeroone.star.setroster.controller;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyNameDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.j8.setroster.SlotReleaseStrategyApis;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.setroster.service.ISlotReleaseStrategyService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：放号策略控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */

@RestController
@Tag(name = "放号策略管理")
@RequiredArgsConstructor
public class SlotReleaseStrategyController implements SlotReleaseStrategyApis {

    private final ISlotReleaseStrategyService strategyService;

    @Operation(summary = "查询放号策略列表")
    @GetMapping("j8/setroster/querySlotReleaseStrategies")
    @Override
    public JsonVO<PageDTO<SlotReleaseStrategyDTO>> querySlotReleaseStrategies(
            @Valid @ModelAttribute SlotReleaseStrategyQuery query) {
        return JsonVO.success(strategyService.listStrategies(query));
    }

    @Operation(summary = "获取放号策略名称")
    @GetMapping("j8/setroster/querySlotReleaseStrategyNames")
    @Override
    public JsonVO<List<SlotReleaseStrategyNameDTO>> querySlotReleaseStrategyNames() {
        return JsonVO.success(strategyService.listStrategyNames());
    }

    @Operation(summary = "新增放号策略")
    @PostMapping("j8/setroster/addSlotReleaseStrategy")
    @Override
    public JsonVO<String> addSlotReleaseStrategy(@Valid @RequestBody SlotReleaseStrategyAddDTO addDTO) {
        return JsonVO.success(strategyService.saveStrategy(addDTO));
    }

    @Operation(summary = "修改放号策略")
    @PutMapping("j8/setroster/modifySlotReleaseStrategy")
    @Override
    public JsonVO<String> modifySlotReleaseStrategy(@Valid @RequestBody SlotReleaseStrategyModifyDTO modifyDTO) {
        if (!strategyService.updateStrategy(modifyDTO)) {
            return JsonVO.fail("放号策略不存在，或数据已被其他人修改");
        }
        return JsonVO.success("修改成功");
    }

    @Operation(summary = "删除放号策略")
    @DeleteMapping("j8/setroster/deleteSlotReleaseStrategy/{id}")
    @Override
    public JsonVO<String> deleteSlotReleaseStrategy(
            @PathVariable("id") String id) {
        if (!strategyService.removeStrategy(id)) {
            return JsonVO.fail("放号策略不存在，或数据已被其他人删除");
        }
        return JsonVO.success("删除成功");
    }
}
