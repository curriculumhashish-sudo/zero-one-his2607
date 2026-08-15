package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.BodyPartBusinessException;
import com.zeroone.star.basedata.service.BodyPartService;
import com.zeroone.star.basedata.service.IBodyPartService;
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

    private final IBodyPartService bodyPartService2;

    @Override
    @Operation(
            summary = "获取部位名称树",
            description = """
            成功：code=10000，data 返回部位树列表
            失败：code=401 未登录或TOKEN过期
                  code=403 无权限
                  code=9999 查询部位树失败
            """
    )
    @GetMapping("/tree")
    public JsonVO<List<BodyPartTreeVO>> getTree() {
        try {
            return JsonVO.success(bodyPartService2.getTree());
        } catch (Exception e) {
            return JsonVO.fail("查询部位树失败：" + e.getMessage());
        }
    }

    @Override
    @Operation(
            summary = "获取部位列表（条件+分页）",
            description = """
            查询参数：
              keyword  部位编码/名称模糊匹配，可空
              parentId 0=查询顶级；大于0=查询指定父级子节点；可空
              status   ACTIVE=启用 INACTIVE=停用，可空
              pageIndex 当前页码，默认1
              pageSize  每页条数，默认20
            成功：code=10000，data 返回 PageDTO（rows 为当前页数据）
            失败：code=401 未登录或TOKEN过期
                  code=403 无权限
                  code=9995 参数异常（pageIndex/pageSize 不合法等）
                  code=9999 查询部位列表失败
            """
    )
    @GetMapping("/list")
    public JsonVO<PageDTO<BodyPartListVO>> getList(@Valid BodyPartPageQuery query) {
        try {
            return JsonVO.success(bodyPartService2.getList(query));
        } catch (Exception e) {
            return JsonVO.fail("查询部位列表失败：" + e.getMessage());
        }
    }

    @Override
    @Operation(
            summary = "添加部位",
            description = """
            请求体：
              partName 部位名称，必填
              parentId 上级部位ID，可空；空表示顶级
            成功：code=10000，data 返回新增部位ID
            失败：code=401 未登录或TOKEN过期
                  code=403 无权限
                  code=9999 部位名称为空，或添加部位失败
            """
    )
    @PostMapping
    public JsonVO<String> add(@Valid @RequestBody BodyPartAddDTO dto) {
        if (dto.getPartName() == null || dto.getPartName().trim().isEmpty()) {
            return JsonVO.fail("部位名称不能为空");
        }
        try {
            return JsonVO.success(bodyPartService2.add(dto));
        } catch (Exception e) {
            return JsonVO.fail("添加部位失败：" + e.getMessage());
        }
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
