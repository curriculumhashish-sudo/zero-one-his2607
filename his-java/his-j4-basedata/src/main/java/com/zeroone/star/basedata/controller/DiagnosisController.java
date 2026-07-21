package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.entity.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：诊断管理控制器（包含所有核心诊断业务功能）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/diagnosis")
@Tag(name = "diagnosis", description = "诊断管理")
public class DiagnosisController implements DiagnosisApis {

    @Resource
    private IDiagnosisService definitionService;

    @Operation(summary = "获取诊断列表（条件+分页）")
    @PostMapping("/query-definition")
    @Override
    public JsonVO<PageDTO<DiagnosisDefinitionVO>> queryDefinitions(@RequestBody DiagnosisDefinitionQuery query) {
        return null;
    }

    @Operation(summary = "获取诊断详情")
    @GetMapping("/get-definition/{id}")
    @Override
    public JsonVO<DiagnosisDefinitionDetailVO> queryDefinitionDetail(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "新增诊断")
    @PostMapping("/add-definition")
    @Override
    public JsonVO<String> addDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        return null;
    }

    @Operation(summary = "修改诊断")
    @PutMapping("/modify-definition")
    @Override
    public JsonVO<String> modifyDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        return null;
    }

    @Operation(summary = "启/停用诊断（支持批量）")
    @PutMapping("/modify-batch-status")
    @Override
    public JsonVO<String> modifyBatchStatus(@RequestBody List<String> ids, @RequestParam String status) {
        return null;
    }

    @Operation(summary = "删除诊断（支持批量）")
    @DeleteMapping("/remove-batch")
    @Override
    public JsonVO<String> removeBatch(@RequestBody List<String> ids) {
        return null;
    }

    @Operation(summary = "获取选择诊断列表（条件+分页）")
    @PostMapping("/get-select-list")
    @Override
    public JsonVO<PageDTO<DiagnosisDefinitionVO>> getSelectList(@RequestBody DiagnosisDefinitionQuery query) {
        return null;
    }
}