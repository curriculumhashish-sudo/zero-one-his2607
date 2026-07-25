package com.zeroone.star.configuration.mapper;

import com.zeroone.star.configuration.entity.EmrMetadataFieldDef;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import org.apache.ibatis.annotations.*;

/**
 * 对于元数据字段的增删改处理的mapper
 * 作者：青梅不甜
 */
@Mapper
public interface FieldMapper {
    /**
     * 添加元数据字段
     * @param data 元数据字段添加 DTO
     * @return 返回受影响行数
     */

    int addField(EmrMetadataFieldDef data);
    /**
     * 修改元数据字段
     * @param data 元数据字段修改 DTO
     * @return 返回受影响行数
     */
    int updateField(FieldUpdateDTO data);

    /**
     * 删除元数据字段
     * @param id 元数据字段 ID
     * @return 返回受影响行数
     */
    @Delete("delete from emr_metadata_field_def where id = #{id}")
    int deleteField(String id);

}
