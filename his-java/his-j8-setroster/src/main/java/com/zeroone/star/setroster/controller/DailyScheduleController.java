package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.j8.setroster.DailyScheduleApis;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleOptionVO;
import com.zeroone.star.setroster.service.IBkgScheduleTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;
import java.util.List;

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
@Tag(name = "日排班模板接口")
@Validated
public class DailyScheduleController implements DailyScheduleApis {

    @Resource
    private IBkgScheduleTemplateService service;

    private static final String TEMPLATE_NOT_FOUND_MSG = "日排班模板不存在或已删除";

    @Override
    @Operation(summary = "获取日排班模板下拉选项")
    @GetMapping("/listDailyScheduleOptions")
    public JsonVO<List<DailyScheduleOptionVO>> listDailyScheduleOptions(@RequestParam(required = false) String organizationId) {
        return JsonVO.success(service.listOptions(organizationId));
    }

    @Override
    @Operation(summary = "分页查询日排班模板列表")
    @GetMapping("/pageDailySchedule")
    public JsonVO<PageDTO<DailyScheduleDTO>> pageDailySchedule(@Valid DailyScheduleQuery query) {
        return JsonVO.success(service.pageDailySchedule(query));
    }

    @Override
    @Operation(summary = "新增日排班模板")
    @PostMapping("/addDailySchedule")
    public JsonVO<String> addDailySchedule(@Valid @RequestBody AddDailyScheduleDTO dto) {
        return JsonVO.success(service.addDailySchedule(dto));
    }

    @Override
    @Operation(summary = "编辑日排班模板")
    @PutMapping("/modifyDailySchedule")
    public JsonVO<String> modifyDailySchedule(@Valid @RequestBody DailyScheduleDTO dto) {
        String result = service.modifyDailySchedule(dto);
        return result != null ? JsonVO.success(result) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }

    @Override
    @Operation(summary = "删除日排班模板")
    @DeleteMapping("/deleteDailySchedule/{id}")
    public JsonVO<String> deleteDailySchedule(@PathVariable String id) {
        String result = service.deleteDailySchedule(id);
        return result != null ? JsonVO.success(result) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }
}
