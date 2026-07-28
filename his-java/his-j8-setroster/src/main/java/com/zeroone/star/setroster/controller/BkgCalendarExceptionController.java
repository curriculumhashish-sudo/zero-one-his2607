package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionAddDto;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionUpdateDto;
import com.zeroone.star.project.j8.setroster.CalendarExceptionApi;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.CalendarExceptionVo;
import com.zeroone.star.setroster.service.IBkgCalendarExceptionService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * T3 日历例外（07 §5.4）前端控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@RestController
@Tag(name = "节日例外管理")
@RequestMapping("/j8/setroster")
@RequiredArgsConstructor
public class BkgCalendarExceptionController implements CalendarExceptionApi {

    private final IBkgCalendarExceptionService service;

    /**
     * 查询节日例外列表
     *
     * @return 包含所有节日例外信息的列表
     */
    @Operation(summary = "查询节日例外列表")
    @GetMapping("/queryCalendarExceptions")
    @Override
    public JsonVO<List<CalendarExceptionVo>> queryAll() {
        return JsonVO.success(service.queryAll());
    }

    /**
     * 新增节日例外
     *
     * @param data 新增数据传输对象，包含类型、日期范围等信息
     * @return 操作结果消息（含新增记录ID）
     */
    @Operation(summary = "新增节日例外")
    @PostMapping("/addCalendarException")
    @Override
    public JsonVO<String> add(@Valid @RequestBody CalendarExceptionAddDto data) {
        return JsonVO.success(service.add(data));
    }

    /**
     * 修改节日例外
     *
     * @param data 更新数据传输对象，必须包含主键ID
     * @return 操作结果消息（含修改记录ID）
     */
    @Operation(summary = "修改节日例外")
    @PutMapping("/modifyCalendarException")
    @Override
    public JsonVO<String> modify(@Valid @RequestBody CalendarExceptionUpdateDto data) {
        return JsonVO.success(service.modify(data));
    }

    /**
     * 删除节日例外（逻辑删除）
     *
     * @param id 节日例外的主键ID
     * @return 操作结果消息（含被删除记录ID）
     */
    @Operation(summary = "删除节日例外")
    @DeleteMapping("/deleteCalendarException/{id}")
    @Override
    public JsonVO<String> delete(@PathVariable String id) {
        return JsonVO.success(service.delete(id));
    }
}
