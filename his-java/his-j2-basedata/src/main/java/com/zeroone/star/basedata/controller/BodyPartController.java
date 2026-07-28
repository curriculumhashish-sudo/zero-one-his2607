package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.BodyPartBusinessException;
import com.zeroone.star.basedata.service.BodyPartService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartStatusDTO;
import com.zeroone.star.project.j2.basedata.bodypart.BodyPartApis;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.JsonVO;

import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

/**
 * 部位管理 Controller
 */
@RestController
@RequestMapping("/basedata/body-part")
@Tag(name = "部位管理")
@RequiredArgsConstructor
public class BodyPartController implements BodyPartApis {

    private final BodyPartService bodyPartService;

    @Override
    @Operation(summary = "获取部位名称树")
    @GetMapping("/tree")
    public JsonVO<List<BodyPartTreeVO>> getTree() {
        return JsonVO.success(new ArrayList<>());
    }

    @Override
    @Operation(summary = "获取部位列表（条件+分页）")
    @GetMapping("/list")
    public JsonVO<PageDTO<BodyPartListVO>> getList(@Valid BodyPartPageQuery query) {
        return JsonVO.success(new PageDTO<>());
    }

    @Override
    @Operation(summary = "添加部位")
    @PostMapping
    public JsonVO<String> add(@Valid @RequestBody BodyPartAddDTO dto) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "修改部位", description = "修改名称、上级部位、拼音码和五笔码")
    @PutMapping("/modify-body-part")
    public JsonVO<Void> modifyBodyPart(@Valid @RequestBody BodyPartModifyDTO dto) {
        try {
            bodyPartService.modifyBodyPart(dto);
            return JsonVO.success(null);
        } catch (BodyPartBusinessException exception) {
            return JsonVO.fail(exception.getMessage());
        }
    }

    @Override
    @Operation(summary = "删除部位", description = "逻辑删除指定部位")
    @DeleteMapping("/remove-body-part/{id}")
    public JsonVO<Void> removeBodyPart(@PathVariable String id) {
        try {
            bodyPartService.removeBodyPart(id);
            return JsonVO.success(null);
        } catch (BodyPartBusinessException exception) {
            return JsonVO.fail(exception.getMessage());
        }
    }

    @Override
    @Operation(summary = "启/停用部位", description = "更新部位启停状态")
    @PutMapping("/modify-body-part-status")
    public JsonVO<Void> modifyBodyPartStatus(@Valid @RequestBody BodyPartStatusDTO dto) {
        try {
            bodyPartService.modifyBodyPartStatus(dto);
            return JsonVO.success(null);
        } catch (BodyPartBusinessException exception) {
            return JsonVO.fail(exception.getMessage());
        }
    }
}
