package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.DocumentTypeService;
import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.j5.config.DocumentTypeApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * 文书类型控制器
 */
@RestController
@RequestMapping("/config/document-type")
@RequiredArgsConstructor
@Tag(name = "配置管理-文书管理", description = "文书管理接口")
@Validated
public class DocumentTypeController implements DocumentTypeApis {
    
    private final DocumentTypeService documentTypeService;
    
    @Override
    @PostMapping("/add")
    public JsonVO<Void> add(@RequestBody @Valid DocumentTypeAddDTO dto) {
        return documentTypeService.add(dto);
    }
    
    @Override
    @PutMapping("/update")
    public JsonVO<Void> update(@RequestBody @Valid DocumentTypeUpdateDTO dto) {
        return documentTypeService.update(dto);
    }
    
    @Override
    @DeleteMapping("/delete/{id}")
    public JsonVO<Void> delete(@PathVariable("id") @NotBlank(message = "ID不能为空") String id) {
        return documentTypeService.delete(id);
    }
}
