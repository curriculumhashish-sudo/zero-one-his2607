package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.lang.Snowflake;
import com.zeroone.star.configuration.entity.EmrMetadataFieldDef;
import com.zeroone.star.configuration.mapper.FieldMapper;
import com.zeroone.star.configuration.service.FieldService;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class FieldServiceImpl implements FieldService {

    @Autowired
    private FieldMapper fieldMapper;

    /**
     *
     * @param data 元数据字段添加 DTO
     * @return  如果数据符合要求就返回JsonVO<String>，否则返回NULL
     */
    @Override
    public int addField(FieldAddDTO data) {
        if(data.getFieldKey() == null && data.getFieldKey().isEmpty()){
            return 0;
        }
        if(data.getLabel() == null || data.getLabel().isEmpty()){
            return 0;
        }
        if(data.getValueSource() == null || data.getValueSource().isEmpty()){
            return 0;
        }
        String newId = new Snowflake(4,18).nextIdStr();
        EmrMetadataFieldDef entity = new EmrMetadataFieldDef();
        BeanUtils.copyProperties(data, entity);
        entity.setId(newId);
        // MP内置insert，自动填充createdAt、updatedAt
        return fieldMapper.addField(entity);
    }

    @Override
    public int updateField(FieldUpdateDTO data) {
        if(data.getFieldKey() == null && data.getFieldKey().isEmpty()){
            return 0;
        }
        if(data.getLabel() == null || data.getLabel().isEmpty()){
            return 0;
        }
        if(data.getValueSource() == null || data.getValueSource().isEmpty()){
            return 0;
        }
        return fieldMapper.updateField(data);
    }

    @Override
    public int deleteField(String id) {
        return fieldMapper.deleteField(id);
    }
}
