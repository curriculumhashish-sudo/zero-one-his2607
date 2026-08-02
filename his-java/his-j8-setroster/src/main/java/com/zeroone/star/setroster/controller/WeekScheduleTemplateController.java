package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.j8.setroster.WeekScheduleTemplateApis;
import com.zeroone.star.project.query.j8.setroster.WeekScheduleTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;
import com.zeroone.star.setroster.service.WeekScheduleTemplateService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

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

    @Resource
    private WeekScheduleTemplateService weekScheduleTemplateService;

    private static final String TEMPLATE_NOT_FOUND_MSG = "周排班模板不存在或已删除";

    @Operation(summary = "分页查询周排班模板列表", description = "支持按关键词、机构/科室ID、启用状态筛选，返回分页结果")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "查询成功"),
            @ApiResponse(responseCode = "400", description = "请求参数错误")
    })
    @GetMapping("/pageWeekScheduleTemplate")
    @Override
    public JsonVO<PageDTO<WeekScheduleTemplateDTO>> pageWeekScheduleTemplate(
            @Valid @ModelAttribute WeekScheduleTemplateQuery query) {
        return JsonVO.success(weekScheduleTemplateService.page(query));
    }

    @Operation(summary = "查询周排班模板详情", description = "根据ID查询周排班模板详情，包含周一~周日明细项")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "查询成功"),
            @ApiResponse(responseCode = "404", description = "模板不存在或已删除")
    })
    @GetMapping("/getWeekScheduleTemplate")
    @Override
    public JsonVO<WeekScheduleTemplateDetailVO> getWeekScheduleTemplate(
            @Parameter(description = "周排班模板ID", required = true, example = "wk-tpl-001")
            @RequestParam("id") String id) {
        WeekScheduleTemplateDetailVO result = weekScheduleTemplateService.getDetail(id);
        return result != null ? JsonVO.success(result) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }

    @Operation(summary = "新增周排班模板", description = "新增周排班模板，包含模板基本信息和周一~周日明细项。模板编码在同机构下需唯一")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "新增成功，返回模板ID"),
            @ApiResponse(responseCode = "400", description = "请求参数错误或模板编码已存在")
    })
    @PostMapping("/addWeekScheduleTemplate")
    @Override
    public JsonVO<String> addWeekScheduleTemplate(
            @Valid @RequestBody AddWeekScheduleTemplateDTO dto) {
        String result = weekScheduleTemplateService.add(dto);
        return result != null ? JsonVO.success(result) : JsonVO.fail("新增失败");
    }

    @Operation(summary = "修改周排班模板", description = "修改周排班模板。明细项采用逐条对比策略：有id的更新，无id的新增，未传入的删除")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "修改成功，返回模板ID"),
            @ApiResponse(responseCode = "400", description = "请求参数错误或模板编码已存在"),
            @ApiResponse(responseCode = "404", description = "模板不存在或已删除")
    })
    @PutMapping("/modifyWeekScheduleTemplate")
    @Override
    public JsonVO<String> modifyWeekScheduleTemplate(
            @Valid @RequestBody WeekScheduleTemplateDTO dto) {
        String result = weekScheduleTemplateService.edit(dto);
        return result != null ? JsonVO.success(result) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }

    @Operation(summary = "删除周排班模板", description = "软删除周排班模板及其明细项")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "删除成功，返回模板ID"),
            @ApiResponse(responseCode = "404", description = "模板不存在或已删除")
    })
    @DeleteMapping("/deleteWeekScheduleTemplate/{id}")
    @Override
    public JsonVO<String> deleteWeekScheduleTemplate(
            @Parameter(description = "周排班模板ID", required = true, example = "wk-tpl-001")
            @PathVariable("id") String id) {
        boolean result = weekScheduleTemplateService.delete(id);
        return result ? JsonVO.success(id) : JsonVO.fail(TEMPLATE_NOT_FOUND_MSG);
    }
}
