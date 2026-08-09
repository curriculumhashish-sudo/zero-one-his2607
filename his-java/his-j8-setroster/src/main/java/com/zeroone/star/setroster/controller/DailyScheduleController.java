package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.j8.setroster.DailyScheduleApis;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleIdVO;
import com.zeroone.star.project.vo.j8.setroster.DailySchedulePageVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleVO;
import com.zeroone.star.setroster.service.IBkgScheduleTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：日排班模板接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@RestController
@RequestMapping("/j8/setroster") // 统一模块路径前缀
@Tag(name = "日排班模板管理")
@Validated
public class DailyScheduleController implements DailyScheduleApis {

    @Resource
    private IBkgScheduleTemplateService service;

    private static final String TEMPLATE_NOT_FOUND_MSG = "日排班模板不存在或已删除";

    @Override
    @Operation(summary = "获取日排班名称")
    @GetMapping("/listDailyScheduleOptions")
    public JsonVO<DailySchedulePageVO<DailyScheduleVO>> listDailyScheduleOptions(@Valid @ModelAttribute DailyScheduleQuery query) {
        return JsonVO.success(service.listDailyScheduleOptions(query));
    }

    @Override
    @Operation(summary = "获取日排班列表（条件+分页）")
    @GetMapping("/pageDailySchedule")
    public JsonVO<DailySchedulePageVO<DailyScheduleVO>> pageDailySchedule(@Valid @ModelAttribute DailyScheduleQuery query) {
        return JsonVO.success(service.pageDailySchedule(query));
    }

    @Override
    @Operation(summary = "新建日排班")
    @PostMapping("/addDailySchedule")
    public JsonVO<DailyScheduleIdVO> addDailySchedule(@Valid @RequestBody AddDailyScheduleDTO dto) {
        String id = service.addDailySchedule(dto);
        return id != null ? JsonVO.success(new DailyScheduleIdVO(id)) : JsonVO.fail("新增日排班模板失败");
    }

    @Override
    @Operation(summary = "修改日排班")
    @PutMapping("/modifyDailySchedule")
    public JsonVO<DailyScheduleIdVO> modifyDailySchedule(@Valid @RequestBody DailyScheduleDTO dto) {
        String id = service.modifyDailySchedule(dto);
        return id != null ? JsonVO.success(new DailyScheduleIdVO(id)) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }

    @Override
    @Operation(summary = "删除日排班")
    @DeleteMapping("/deleteDailySchedule/{id}")
    public JsonVO<DailyScheduleIdVO> deleteDailySchedule(@PathVariable String id) {
        String result = service.deleteDailySchedule(id);
        return result != null ? JsonVO.success(new DailyScheduleIdVO(result)) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }
}
