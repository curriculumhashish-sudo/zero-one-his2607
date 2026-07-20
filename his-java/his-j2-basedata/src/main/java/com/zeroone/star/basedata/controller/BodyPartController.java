package com.zeroone.star.basedata.controller;


import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartStatusDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j2.basedata.bodypart.BodyPartApis;

import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;

import com.zeroone.star.project.vo.j2.basedata.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.BodyPartTreeVO;
import com.zeroone.star.project.vo.JsonVO;
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
    @Override
    @Operation(summary = "获取部位名称树")
    @GetMapping("/tree")
    public JsonVO<List<BodyPartTreeVO>> getTree() {
        // TODO 后续实现从数据库获取部位树
        return JsonVO.success(new ArrayList<>());
    }

    @Override
    @Operation(summary = "获取部位列表（条件+分页）")
    @GetMapping("/list")
    public JsonVO<PageDTO<BodyPartListVO>> getList(@Valid BodyPartPageQuery query) {
        // TODO 后续实现分页条件查询
        return JsonVO.success(new PageDTO<>());
    }

    @Override
    @Operation(summary = "添加部位")
    @PostMapping
    public JsonVO<String> add(@Valid @RequestBody BodyPartAddDTO dto) {
        // TODO 后续实现新增部位
        return JsonVO.success(null);
    }

    /**
     * 修改部位名称、父级和输入码。后续业务层需校验父级不能为自身或子节点。
     */
    @Override
    @Operation(summary = "修改部位", description = "修改名称、上级部位、拼音码和五笔码")
    @PutMapping("/modify-body-part")
    public JsonVO<Void> modifyBodyPart(@Valid @RequestBody BodyPartModifyDTO dto) {
        // TODO 后续实现存在性、层级循环和乐观锁校验。
        return JsonVO.success(null);
    }

    /**
     * 软删除部位。后续业务层需确认不存在未删除的子部位。
     */
    @Override
    @Operation(summary = "删除部位", description = "逻辑删除指定部位")
    @DeleteMapping("/remove-body-part/{id}")
    public JsonVO<Void> removeBodyPart(@PathVariable String id) {
        // TODO 后续实现子节点校验和软删除。
        return JsonVO.success(null);
    }

    /**
     * 将部位状态设置为 ACTIVE 或 INACTIVE。
     */
    @Override
    @Operation(summary = "启/停用部位", description = "更新部位启停状态")
    @PutMapping("/modify-body-part-status")
    public JsonVO<Void> modifyBodyPartStatus(@Valid @RequestBody BodyPartStatusDTO dto) {
        // TODO 后续实现存在性和乐观锁校验；相同状态直接返回成功。
        return JsonVO.success(null);
    }
}
