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
        try {
            com.baomidou.mybatisplus.extension.plugins.pagination.Page<ClnConditionDefinitionDO> page
                    = definitionService.pageQuery(query);
            return JsonVO.success(PageDTO.create(page, DiagnosisDefinitionVO::new));
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "获取诊断详情")
    @GetMapping("/get-definition/{id}")
    @Override
    public JsonVO<DiagnosisDefinitionDetailVO> queryDefinitionDetail(@PathVariable String id) {
        try {
            ClnConditionDefinitionDO entity = definitionService.getById(id);
            if (entity == null) {
                return JsonVO.create(null, 404, "诊断定义不存在");
            }

            DiagnosisDefinitionDetailVO vo = new DiagnosisDefinitionDetailVO();
            vo.setId(entity.getId());
            vo.setSourceEnum(entity.getSourceEnum());
            vo.setConditionCode(entity.getConditionCode());
            vo.setName(entity.getName());
            vo.setPyStr(entity.getPyStr());
            vo.setWbStr(entity.getWbStr());
            vo.setTypeCode(entity.getTypeCode());
            vo.setDiseaseCode(entity.getDiseaseCode());
            vo.setYbFlag(entity.getYbFlag());
            vo.setYbNo(entity.getYbNo());
            vo.setYbMatchFlag(entity.getYbMatchFlag());
            vo.setStatusEnum(entity.getStatusEnum());
            vo.setDescription(entity.getDescription());

            List<DiagnosisDefinitionDetailVO.ReferenceDetail> refs = definitionService.getReferences(id);
            vo.setReferences(refs);

            return JsonVO.success(vo);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "新增诊断")
    @PostMapping("/add-definition")
    @Override
    public JsonVO<String> addDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        try {
            String id = definitionService.saveDefinition(dto);
            return JsonVO.success(id);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "修改诊断")
    @PutMapping("/modify-definition")
    @Override
    public JsonVO<String> modifyDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        try {
            definitionService.modifyDefinition(dto);
            return JsonVO.success("编辑成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "启/停用诊断（支持批量）")
    @PutMapping("/batch-modify-status")
    @Override
    public JsonVO<String> modifyBatchStatus(@RequestBody List<String> ids, @RequestParam String status) {
        try {
            definitionService.modifyBatchStatus(ids, status);
            return JsonVO.success("操作成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "删除诊断（支持批量）")
    @DeleteMapping("/batch-delete")
    @Override
    public JsonVO<String> removeBatch(@RequestBody List<String> ids) {
        try {
            definitionService.removeBatch(ids);
            return JsonVO.success("删除成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "获取选择诊断列表（条件+分页）")
    @PostMapping("/get-select-list")
    @Override
    public JsonVO<PageDTO<DiagnosisDefinitionVO>> getSelectList(@RequestBody DiagnosisDefinitionQuery query) {
        try {
            com.baomidou.mybatisplus.extension.plugins.pagination.Page<ClnConditionDefinitionDO> page
                    = definitionService.getSelectList(query);
            return JsonVO.success(PageDTO.create(page, DiagnosisDefinitionVO::new));
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }
}