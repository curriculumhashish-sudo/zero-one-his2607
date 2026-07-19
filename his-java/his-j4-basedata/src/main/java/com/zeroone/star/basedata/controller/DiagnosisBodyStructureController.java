package com.zeroone.star.diagnosis.controller;

import com.zeroone.star.diagnosis.service.IDiaBodyStructureService;
import com.zeroone.star.project.dto.j4.diagnosis.BodyStructureDTO;
import com.zeroone.star.project.j4.diagnosis.DiagnosisApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.BodyStructureTreeNodeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：身体结构目录控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@Tag(name = "diagnosis-body-structure", description = "身体结构目录管理")
public class DiagnosisBodyStructureController implements DiagnosisApis {

    @Resource
    private IDiaBodyStructureService bodyStructureService;

    @Operation(summary = "查询身体结构树")
    @GetMapping("query-body-structure")
    @Override
    public JsonVO<List<BodyStructureTreeNodeVO>> queryBodyStructureTree() {
        try {
            List<BodyStructureTreeNodeVO> tree = bodyStructureService.buildTree();
            return JsonVO.success(tree);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "新增身体结构节点")
    @PostMapping("add-body-structure")
    @Override
    public JsonVO<String> addBodyStructureNode(@RequestBody BodyStructureDTO dto) {
        try {
            String id = bodyStructureService.saveNode(dto);
            return JsonVO.success(id);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "编辑身体结构节点")
    @PutMapping("modify-body-structure")
    @Override
    public JsonVO<String> modifyBodyStructureNode(@RequestBody BodyStructureDTO dto) {
        try {
            bodyStructureService.modifyNode(dto);
            return JsonVO.success("编辑成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "删除身体结构节点")
    @DeleteMapping("delete-body-structure/{id}")
    @Override
    public JsonVO<String> deleteBodyStructureNode(@PathVariable String id) {
        try {
            bodyStructureService.deleteNode(id);
            return JsonVO.success("删除成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }
}
