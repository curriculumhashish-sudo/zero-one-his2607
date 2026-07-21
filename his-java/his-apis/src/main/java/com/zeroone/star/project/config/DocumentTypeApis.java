package com.zeroone.star.project.config;

import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * 文书类型API接口
 */
@Tag(name = "文书类型管理", description = "文书类型的增删改操作")
public interface DocumentTypeApis {
    
    /**
     * 新增文书类型
     */
    @Operation(summary = "新增文书类型", description = "创建新的文书类型记录")
    @PostMapping("/add")
    JsonVO<Void> add(@RequestBody DocumentTypeAddDTO dto);
    
    /**
     * 修改文书类型
     */
    @Operation(summary = "修改文书类型", description = "更新文书类型信息")
    @PutMapping("/update")
    JsonVO<Void> update(@RequestBody DocumentTypeUpdateDTO dto);
    
    /**
     * 删除文书类型
     */
    @Operation(summary = "删除文书类型", description = "软删除文书类型记录")
    @DeleteMapping("/delete/{id}")
    JsonVO<Void> delete(@PathVariable("id") String id);
}
