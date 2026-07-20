package com.zeroone.star.setroster.controller;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDetailDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.j8.setroster.SlotReleaseStrategyApis;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

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
public class SlotReleaseStrategyController implements SlotReleaseStrategyApis {


    @Operation(summary = "查询放号策略列表")
    @GetMapping("j8/setroster/querySlotReleaseStrategies")
    @Override
    public JsonVO<PageDTO<SlotReleaseStrategyDTO>> querySlotReleaseStrategies(
            @Valid @ModelAttribute SlotReleaseStrategyQuery query) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "查询放号策略详情")
    @GetMapping("j8/setroster/getSlotReleaseStrategy")
    @Override
    public JsonVO<SlotReleaseStrategyDetailDTO> getSlotReleaseStrategy(
            @RequestParam("id") Long id) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "新增放号策略")
    @PostMapping("j8/setroster/addSlotReleaseStrategy")
    @Override
    public JsonVO<String> addSlotReleaseStrategy(@Valid @RequestBody SlotReleaseStrategyAddDTO addDTO) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "修改放号策略")
    @PutMapping("j8/setroster/modifySlotReleaseStrategy")
    @Override
    public JsonVO<String> modifySlotReleaseStrategy(@Valid @RequestBody SlotReleaseStrategyModifyDTO modifyDTO) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }

    @Operation(summary = "删除放号策略")
    @DeleteMapping("j8/setroster/deleteSlotReleaseStrategy/{id}")
    @Override
    public JsonVO<String> deleteSlotReleaseStrategy(
            @PathVariable("id") Long id) {
        return JsonVO.create(null, ResultStatus.API_UN_IMPL);
    }
}
