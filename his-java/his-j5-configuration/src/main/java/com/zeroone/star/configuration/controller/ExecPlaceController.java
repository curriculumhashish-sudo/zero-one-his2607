package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.emr.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.project.emr.configmanage.query.ExecPlaceQuery;
import com.zeroone.star.project.emr.configmanage.vo.ExecPlaceVO;
import com.zeroone.star.configuration.service.IExecPlaceService;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/config/exec-place")
@Tag(name = "配置管理-执行地点", description = "执行地点增删改查接口")
public class ExecPlaceController {

    @Resource
    private IExecPlaceService execPlaceService;

    @Operation(summary = "新增执行地点")
    @PostMapping("/add")
    public JsonVO<String> add(@RequestBody ExecPlaceDTO execPlaceDTO) {
        return execPlaceService.add(execPlaceDTO);
    }

    @Operation(summary = "修改执行地点")
    @PutMapping("/update")
    public JsonVO<String> update(@RequestBody ExecPlaceDTO execPlaceDTO) {
        return execPlaceService.update(execPlaceDTO);
    }

    @Operation(summary = "删除执行地点")
    @DeleteMapping("/delete/{id}")
    public JsonVO<String> delete(@PathVariable("id") Long id) {
        return execPlaceService.delete(id);
    }

    @Operation(summary = "分页查询执行地点列表")
    @GetMapping("/query")
    public JsonVO<PageDTO<ExecPlaceVO>> queryByPage(ExecPlaceQuery query) {
        return execPlaceService.queryByPage(query);
    }

    @Operation(summary = "根据ID查询执行地点详情")
    @GetMapping("/query/{id}")
    public JsonVO<ExecPlaceVO> queryById(@PathVariable("id") Long id) {
        return execPlaceService.queryById(id);
    }
}
