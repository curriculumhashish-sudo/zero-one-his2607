package com.zeroone.star.project.j5.emr.template;

import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * 病历模板写操作接口。
 */
@Tag(name = "配置管理-病历模板", description = "病历模板配置管理接口")
@RequestMapping("/medical-template")
public interface MedicalTemplateWriteApis {

    @Operation(summary = "新增病历模板", description = "新增模板并返回生成的模板ID")
    @PostMapping
    JsonVO<String> addMedicalTemplate(@Valid @RequestBody MedicalTemplateCreateDTO dto);

    @Operation(summary = "修改病历模板", description = "根据模板ID更新请求中提供的字段")
    @PutMapping("/{id}")
    JsonVO<String> modifyMedicalTemplate(
            @Parameter(description = "模板ID", required = true)
            @PathVariable("id") String id,
            @Valid @RequestBody MedicalTemplateUpdateDTO dto);

    @Operation(summary = "删除病历模板", description = "根据模板ID软删除病历模板")
    @DeleteMapping("/{id}")
    JsonVO<String> removeMedicalTemplate(
            @Parameter(description = "模板ID", required = true)
            @PathVariable("id") String id);
}
