package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 文书类型服务接口
 */
public interface DocumentTypeService {
    
    /**
     * 新增文书类型
     */
    JsonVO<Void> add(DocumentTypeAddDTO dto);
    
    /**
     * 修改文书类型
     */
    JsonVO<Void> update(DocumentTypeUpdateDTO dto);
    
    /**
     * 删除文书类型
     */
    JsonVO<Void> delete(String id);
}
