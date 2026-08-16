package com.zeroone.star.project.j5.medicalTemplate;

import com.zeroone.star.project.dto.j5.medicalTemplate.FieldBaseDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;

/**
 * <p>
 * 描述：病历模板字段管理API接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Tag(name = "配置管理-元数据字段", description = "元数据字段管理接口")
public interface FieldApis {

    @Operation(summary = "添加元数据字段", description = "添加新的元数据字段")
    JsonVO<String> addField(@Valid FieldBaseDTO data);

    @Operation(summary = "修改元数据字段", description = "根据ID修改元数据字段")
    JsonVO<String> updateField(@Valid FieldUpdateDTO data);

    @Operation(summary = "删除元数据字段", description = "根据ID删除元数据字段")
    @Parameter(name = "id", description = "字段ID", required = true, in = ParameterIn.PATH)
    JsonVO<String> delField(String id);
}
