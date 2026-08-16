package com.zeroone.star.project.j5.config;

import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：文书类型管理API接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Tag(name = "配置管理-文书管理", description = "文书管理接口")
@Validated
@RequestMapping("/config/document-type")
public interface DocumentTypeApis {

    @Operation(summary = "新增文书类型", description = "新增文书类型")
    @PostMapping("/add")
    JsonVO<Void> add(@RequestBody @Valid DocumentTypeAddDTO dto);

    @Operation(summary = "修改文书类型", description = "根据ID修改文书类型")
    @PutMapping("/update")
    JsonVO<Void> update(@RequestBody @Valid DocumentTypeUpdateDTO dto);

    @Operation(summary = "删除文书类型", description = "根据ID删除文书类型")
    @DeleteMapping("/delete/{id}")
    JsonVO<Void> delete(
            @Parameter(description = "文书类型ID", required = true)
            @NotBlank(message = "ID不能为空")
            @PathVariable("id") String id);
}
