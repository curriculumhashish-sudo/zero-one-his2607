package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.j8.setroster.WeekScheduleTemplateApis;
import com.zeroone.star.project.query.j8.setroster.WeekScheduleTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;

/**
 * <p>
 * 描述：周排班模板控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j8/setroster") // 统一模块路径前缀
@Tag(name = "周排班模板管理")
public class WeekScheduleTemplateController implements WeekScheduleTemplateApis {

    @Operation(summary = "分页查询周排班模板列表")
    @GetMapping("j8/setroster/pageWeekScheduleTemplate")
    @Override
    public JsonVO<PageDTO<WeekScheduleTemplateDTO>> pageWeekScheduleTemplate(
            @Valid @ModelAttribute WeekScheduleTemplateQuery query) {
        return JsonVO.success(new PageDTO<>(1L, 10L, 0L, 0L, new ArrayList<>()));
    }

    @Operation(summary = "查询周排班模板详情")
    @GetMapping("j8/setroster/getWeekScheduleTemplate")
    @Override
    public JsonVO<WeekScheduleTemplateDetailVO> getWeekScheduleTemplate(
            @RequestParam("id") Long id) {
        return JsonVO.success(new WeekScheduleTemplateDetailVO());
    }

    @Operation(summary = "新增周排班模板")
    @PostMapping("j8/setroster/addWeekScheduleTemplate")
    @Override
    public JsonVO<String> addWeekScheduleTemplate(
            @Valid @RequestBody AddWeekScheduleTemplateDTO dto) {
        return JsonVO.success("");
    }

    @Operation(summary = "修改周排班模板")
    @PutMapping("j8/setroster/modifyWeekScheduleTemplate")
    @Override
    public JsonVO<String> modifyWeekScheduleTemplate(
            @Valid @RequestBody WeekScheduleTemplateDTO dto) {
        return JsonVO.success("");
    }

    @Operation(summary = "删除周排班模板")
    @DeleteMapping("j8/setroster/deleteWeekScheduleTemplate/{id}")
    @Override
    public JsonVO<String> deleteWeekScheduleTemplate(
            @PathVariable("id") Long id) {
        return JsonVO.success("");
    }
}
