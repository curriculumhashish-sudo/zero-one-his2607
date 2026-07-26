package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.lang.Snowflake;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.zeroone.star.configuration.entity.EmrMetadataFieldDef;
import com.zeroone.star.configuration.mapper.FieldMapper;
import com.zeroone.star.configuration.service.FieldService;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

@Service
public class FieldServiceImpl implements FieldService {

    @Autowired
    private FieldMapper fieldMapper;

    /**
     *
     * @param data 元数据字段添加 DTO
     * @return 返回受影响行数
     */
    @Override
    public int addField(FieldAddDTO data) {
        if (data.getFieldKey() == null || data.getFieldKey().isEmpty()) {
            throw new RuntimeException("FieldKey不能为空");
        }
        if (data.getLabel() == null || data.getLabel().isEmpty()) {
            throw new RuntimeException("显示名不能为空");
        }
        if (data.getValueSource() == null || data.getValueSource().isEmpty()) {
            throw new RuntimeException("取值来源不能为空");
        }
        LambdaQueryWrapper<EmrMetadataFieldDef> checkWrapper = Wrappers.lambdaQuery();
        checkWrapper.eq(EmrMetadataFieldDef::getFieldKey, data.getFieldKey());
        checkWrapper.eq(EmrMetadataFieldDef::getIsDeleted, 0);
        long count = fieldMapper.selectCount(checkWrapper);
        if (count > 0) {
            throw new RuntimeException("fieldKey:" + data.getFieldKey() + "已存在，不可重复新增");
        }
        String newId = new Snowflake(4, 18).nextIdStr();
        EmrMetadataFieldDef entity = new EmrMetadataFieldDef();
        BeanUtils.copyProperties(data, entity);
        entity.setId(newId);
        return fieldMapper.insert(entity);
    }

    /**
     *
     * @param data 元数据字段修改 DTO
     * @return 返回受影响行数
     */
    @Override
    public int updateField(FieldUpdateDTO data) {
        if (data.getFieldKey() == null || data.getFieldKey().isEmpty()) {
            throw new RuntimeException("FieldKey不能为空");
        }
        if (data.getLabel() == null || data.getLabel().isEmpty()) {
            throw new RuntimeException("显示名不能为空");
        }
        if (data.getValueSource() == null || data.getValueSource().isEmpty()) {
            throw new RuntimeException("取值来源不能为空");
        }
        LambdaQueryWrapper<EmrMetadataFieldDef> queryWrapper = Wrappers.lambdaQuery();
        queryWrapper.eq(EmrMetadataFieldDef::getFieldKey, data.getFieldKey());
        queryWrapper.eq(EmrMetadataFieldDef::getIsDeleted, 0);
        EmrMetadataFieldDef oldEntity = fieldMapper.selectOne(queryWrapper);
        if (oldEntity == null) {
            throw new RuntimeException("修改失败：当前fieldKey对应数据不存在或已删除");
        }
        BeanUtils.copyProperties(data, oldEntity,"id");
        oldEntity.setUpdatedAt(LocalDateTime.now());
        return fieldMapper.updateById(oldEntity);
    }

    /**
     *
     * @param id 元数据字段 ID
     * @return 返回受影响行数
     */
    @Override
    public int deleteField(String id) {
        if (id == null || id.isEmpty()) {
            return 0;
        }
        return fieldMapper.deleteById(id);
    }
}