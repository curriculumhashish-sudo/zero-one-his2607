package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.j5.medicalTemplate.FieldBaseDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 元数据字段服务接口
 * 作者：青梅不甜
 */
public interface FieldService {
    /**
     * 添加元数据字段
     * @param data 元数据字段添加 DTO
     * @return 操作结果 JSON 包含 ID 字段
     */
    int addField(FieldBaseDTO data);
    /**
     * 修改元数据字段
     * @param data 元数据字段修改 DTO
     * @return 操作结果 JSON 包含 ID 字段
     */
    int updateField(FieldUpdateDTO data);
    /**
     * 删除元数据字段
     * @param id 元数据字段 ID
     * @return 操作结果 JSON 包含 ID 字段
     */
    int deleteField(String id);

}
