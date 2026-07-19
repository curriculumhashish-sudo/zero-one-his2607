package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.entity.ClnConditionDefinition;
import com.zeroone.star.basedata.service.IClnConditionDefinitionService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.YbMappingDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionVO;
import com.zeroone.star.project.vo.j4.diagnosis.DisableCheckResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：诊断定义控制器（诊断目录维护）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@Tag(name = "diagnosis-definition", description = "诊断目录维护")
public class DiagnosisDefinitionController implements DiagnosisApis {

    @Resource
    private IClnConditionDefinitionService definitionService;

    @Operation(summary = "分页查询诊断定义")
    @PostMapping("query-definition")
    @Override
    public JsonVO<PageDTO<DiagnosisDefinitionVO>> queryDefinitions(@RequestBody DiagnosisDefinitionQuery query) {
        try {
            com.baomidou.mybatisplus.extension.plugins.pagination.Page<ClnConditionDefinition> page
                    = definitionService.pageQuery(query);
            return JsonVO.success(PageDTO.create(page, DiagnosisDefinitionVO::new));
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "查询诊断定义详情（含引用关系）")
    @GetMapping("get-definition/{id}")
    @Override
    public JsonVO<DiagnosisDefinitionDetailVO> queryDefinitionDetail(@PathVariable String id) {
        try {
            ClnConditionDefinition entity = definitionService.getById(id);
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

    @Operation(summary = "新增诊断定义")
    @PostMapping("add-definition")
    @Override
    public JsonVO<String> addDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        try {
            String id = definitionService.saveDefinition(dto);
            return JsonVO.success(id);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "编辑诊断定义")
    @PutMapping("modify-definition")
    @Override
    public JsonVO<String> modifyDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        try {
            definitionService.modifyDefinition(dto);
            return JsonVO.success("编辑成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "停用诊断定义前的引用校验")
    @GetMapping("query-disable-check/{id}")
    @Override
    public JsonVO<DisableCheckResultVO> queryDisableCheck(@PathVariable String id) {
        try {
            DisableCheckResultVO result = definitionService.queryDisableCheck(id);
            return JsonVO.success(result);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "停用诊断定义")
    @PutMapping("modify-disable/{id}")
    @Override
    public JsonVO<String> modifyDisableDefinition(@PathVariable String id) {
        try {
            definitionService.modifyDisableDefinition(id);
            return JsonVO.success("停用成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "医保对照维护")
    @PutMapping("modify-yb-mapping")
    @Override
    public JsonVO<String> updateYbMapping(@RequestBody YbMappingDTO dto) {
        try {
            definitionService.updateYbMapping(dto);
            return JsonVO.success("医保对照维护成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "导出诊断定义")
    @PostMapping("export-definition")
    @Override
    public JsonVO<String> exportDefinitions(@RequestBody DiagnosisDefinitionQuery query) {
        // TODO: 导出 Excel 功能，待后续接入 FesodSheet 实现
        return JsonVO.create(null, 501, "导出功能待实现");
    }
}
