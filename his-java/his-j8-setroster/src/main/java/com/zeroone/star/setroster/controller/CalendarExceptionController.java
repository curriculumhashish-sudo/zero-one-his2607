package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionAddDto;
import com.zeroone.star.project.dto.j8.setroster.CalendarExceptionUpdateDto;
import com.zeroone.star.project.j8.setroster.CalendarExceptionApi;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.CalendarExceptionVo;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>描述: 节日例外管理控制器</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j8/setroster") // 统一模块路径前缀
@Tag(name = "节日例外管理")
public class CalendarExceptionController implements CalendarExceptionApi {

    /**
     * 查询节日例外列表
     * 注意：这里假设使用了通用的 PageQuery 对象来处理分页参数，
     * 如果你的项目中没有 PageQuery，可以暂时用 null 或者自定义一个包含 pageNum/pageSize 的对象
     */
    @Operation(summary = "查询节日例外列表")
    @GetMapping("/querySlotReleaseStrategies") // 路径规范化
    @Override
    public JsonVO<List<CalendarExceptionVo>> queryAll() {
        // TODO: 待实现具体查询逻辑
        return null;
    }

    /**
     * 新增节日例外
     */
    @Operation(summary = "新增节日例外")
    @PostMapping("/addSlotReleaseStrategy") // 路径规范化
    @Override
    public JsonVO<String> add(@RequestBody CalendarExceptionAddDto data) {
        // TODO: 待实现具体新增逻辑
        return null;
    }

    /**
     * 修改节日例外
     */
    @Operation(summary = "修改节日例外")
    @PutMapping("/modifySlotReleaseStrategy") // 修正为 PutMapping，路径规范化
    @Override
    public JsonVO<String> modify(@RequestBody CalendarExceptionUpdateDto data) {
        // TODO: 待实现具体修改逻辑
        return null;
    }

    /**
     * 删除节日例外
     */
    @Operation(summary = "删除节日例外")
    @DeleteMapping("/deleteSlotReleaseStrategy/{id}") // 路径规范化
    @Override
    public JsonVO<String> delete(@PathVariable String id) {
        // TODO: 待实现具体删除逻辑
        return null;
    }

}