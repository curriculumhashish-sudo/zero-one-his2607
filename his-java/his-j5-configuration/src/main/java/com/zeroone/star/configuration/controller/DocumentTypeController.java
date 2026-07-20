package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.DocumentTypeService;
import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.config.DocumentTypeApis;
import com.zeroone.star.project.vo.JsonVO;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

/**
 * 文书类型控制器
 */
@RestController
@RequestMapping("/config/document-type")
@RequiredArgsConstructor
public class DocumentTypeController implements DocumentTypeApis {
    
    private final DocumentTypeService documentTypeService;
    
    @Override
    @PostMapping("/add")
    public JsonVO<Void> add(@RequestBody DocumentTypeAddDTO dto) {
        return documentTypeService.add(dto);
    }
    
    @Override
    @PutMapping("/update")
    public JsonVO<Void> update(@RequestBody DocumentTypeUpdateDTO dto) {
        return documentTypeService.update(dto);
    }
    
    @Override
    @DeleteMapping("/delete/{id}")
    public JsonVO<Void> delete(@PathVariable("id") String id) {
        return documentTypeService.delete(id);
    }
}
