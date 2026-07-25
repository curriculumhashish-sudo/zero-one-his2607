package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IEmrDocumentTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.configuration_management.medical_record_template.EmrDocumentTypeApis;
import com.zeroone.star.project.query.j5.configuration_management.medical_record_template.EmrDocumentTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeDetailVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeNameVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：获取文书类型控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */
@RestController
@Tag(name = "配置管理-文书类型", description = "文书类型接口")
@Validated
@RequestMapping("/emr/document-type")
public class EmrDocumentTypeController implements EmrDocumentTypeApis {

    @Resource
    private IEmrDocumentTypeService emrDocumentTypeService;

    /**
     * 获取文书类型下拉字典（仅启用）
     */
    @Override
    @Operation(summary = "获取文书类型下拉选项")
    @GetMapping("/name")
    public JsonVO<List<EmrDocumentTypeNameVO>> getEmrDocumentTypeName() {
        return JsonVO.success(emrDocumentTypeService.getEmrDocumentTypeName());
    }

    /**
     * 分页查询文书类型列表
     */
    @Override
    @Operation(summary = "分页查询全部文书类型")
    @GetMapping("/page")
    public JsonVO<PageDTO<EmrDocumentTypeVO>> queryEmrDocumentType(@Valid EmrDocumentTypeQuery query) {
        return JsonVO.success(emrDocumentTypeService.queryEmrDocumentType(query));
    }

    /**
     * 根据ID查询单条详情
     */
    @Override
    @Operation(summary = "根据主键ID查询文书类型详情")
    @GetMapping("/{id}")
    @Parameter(name = "id", description = "文书类型主键ID", required = true, in = ParameterIn.PATH, example = "323100000000000101")
    public JsonVO<EmrDocumentTypeDetailVO> getEmrDocumentTypeDetail(@PathVariable("id") String id) {
        return JsonVO.success(emrDocumentTypeService.getEmrDocumentTypeDetail(id));
    }
}
