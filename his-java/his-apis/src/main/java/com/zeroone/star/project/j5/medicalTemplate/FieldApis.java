package com.zeroone.star.project.j5.medicalTemplate;

import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 字段管理接口
 * 作者：青梅不甜
 */
public interface FieldApis {

    /**
     * 添加元数据字段
     * @param data 字段添加对象
     * @return 添加结果
     */
    JsonVO<String> addField(FieldAddDTO data);

    /**
     * 修改元数据字段
     * @param data 字段修改对象
     * @return 修改结果
     */
    JsonVO<String> updateField(FieldUpdateDTO data);

    /**
     * 根据id删除字段
     * @param id 字段ID
     * @return 删除结果
     */
    JsonVO<String> delField(String id);
}
