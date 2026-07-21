package com.zeroone.star.project.controller;

import com.zeroone.star.project.dto.BodyPartAddDTO;
import com.zeroone.star.project.query.BodyPartPageQuery;
import com.zeroone.star.project.vo.BodyPartListVO;
import com.zeroone.star.project.vo.BodyPartTreeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 部位管理 API 接口定义
 */
@Tag(name = "部位管理")
public interface IBodyPartController {

    @Operation(summary = "获取部位名称树")
    @GetMapping("/basedata/body-part/tree")
    List<BodyPartTreeVO> getTree();

    @Operation(summary = "获取部位列表（条件+分页）")
    @GetMapping("/basedata/body-part/list")
    List<BodyPartListVO> getList(@Valid BodyPartPageQuery query);

    @Operation(summary = "添加部位")
    @PostMapping("/basedata/body-part")
    Long add(@Valid @RequestBody BodyPartAddDTO dto);
}