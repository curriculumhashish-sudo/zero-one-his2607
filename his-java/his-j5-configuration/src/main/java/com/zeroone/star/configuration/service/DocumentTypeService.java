package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：文书类型服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
public interface DocumentTypeService {

    /**
     * 新增文书类型
     * @param dto 新增DTO
     * @return 操作结果
     */
    JsonVO<Void> add(DocumentTypeAddDTO dto);

    /**
     * 修改文书类型
     * @param dto 修改DTO
     * @return 操作结果
     */
    JsonVO<Void> update(DocumentTypeUpdateDTO dto);

    /**
     * 删除文书类型
     * @param id 文书类型ID
     * @return 操作结果
     */
    JsonVO<Void> delete(String id);
}
