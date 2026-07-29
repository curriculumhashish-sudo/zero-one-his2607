package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.lang.Snowflake;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.zeroone.star.configuration.entity.EmrMetadataFieldDef;
import com.zeroone.star.configuration.mapper.FieldMapper;
import com.zeroone.star.configuration.service.FieldService;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.exception.BusinessException;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

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
        /**
         * 校验参数
         * 使用同意异常处理
         */
        validate(data.getFieldKey(), data.getLabel(), data.getValueSource(),data.getSortOrder());
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
    // 1. 参数基础校验
    validate(data.getFieldKey(), data.getLabel(), data.getValueSource(),data.getSortOrder());

    // 2. 根据fieldKey查询现有记录（只查id，无select *，性能最优）
    LambdaQueryWrapper<EmrMetadataFieldDef> existWrapper = Wrappers.lambdaQuery();
    existWrapper.select(EmrMetadataFieldDef::getId); // 只查主键，避免select *
    existWrapper.eq(EmrMetadataFieldDef::getFieldKey, data.getFieldKey());
    existWrapper.eq(EmrMetadataFieldDef::getIsDeleted, 0);
    EmrMetadataFieldDef oldEntity = fieldMapper.selectOne(existWrapper);
    if (oldEntity == null) {
        throw new BusinessException("修改失败：fieldKey[" + data.getFieldKey() + "]对应数据不存在或已删除");
    }
    String dbId = oldEntity.getId(); // 数据库查到真实主键

    // 4. 更新逻辑，使用数据库查询出来的dbId作为更新条件
    LambdaUpdateWrapper<EmrMetadataFieldDef> wrapper = Wrappers.lambdaUpdate();
    wrapper.eq(EmrMetadataFieldDef::getId, dbId);

    // 必填字段直接更新
    //wrapper.set(EmrMetadataFieldDef::getFieldKey, data.getFieldKey());
    wrapper.set(EmrMetadataFieldDef::getLabel, data.getLabel());
    wrapper.set(EmrMetadataFieldDef::getValueSource, data.getValueSource());

    // 非必填字段空值判断更新（保留你原有逻辑不变）
    if (StringUtils.hasText(data.getDataType())) {
        wrapper.set(EmrMetadataFieldDef::getDataType, data.getDataType());
    }
    if (data.getEditableDefault() != null) {
        wrapper.set(EmrMetadataFieldDef::getEditableDefault, data.getEditableDefault());
    }
    if (StringUtils.hasText(data.getSourcePath())) {
        wrapper.set(EmrMetadataFieldDef::getSourcePath, data.getSourcePath());
    }
    if (StringUtils.hasText(data.getDefaultValue())) {
        wrapper.set(EmrMetadataFieldDef::getDefaultValue, data.getDefaultValue());
    }
    if (StringUtils.hasText(data.getMaskRule())) {
        wrapper.set(EmrMetadataFieldDef::getMaskRule, data.getMaskRule());
    }
    if (StringUtils.hasText(data.getFormatPattern())) {
        wrapper.set(EmrMetadataFieldDef::getFormatPattern, data.getFormatPattern());
    }
    if (data.getSortOrder() != null) {
        wrapper.set(EmrMetadataFieldDef::getSortOrder, data.getSortOrder());
    }
    if (data.getIsEnabled() != null) {
        wrapper.set(EmrMetadataFieldDef::getIsEnabled, data.getIsEnabled());
    }
    if (StringUtils.hasText(data.getRemark())) {
        wrapper.set(EmrMetadataFieldDef::getRemark, data.getRemark());
    }

    // 自动更新修改时间
    wrapper.set(EmrMetadataFieldDef::getUpdatedAt, LocalDateTime.now());
    return fieldMapper.update(null, wrapper);
    }

    /**
     *
     * @param id 元数据字段 ID
     * @return 返回受影响行数
     */
    @Override
    public int deleteField(String id) {
        if (id == null || id.isEmpty()) {
            throw new BusinessException("ID不能为空");
        }
        return fieldMapper.deleteById(id);
    }

    private void validate(String fieldKey, String label, String valueSource,Integer sortOrder) {
        if (fieldKey == null || fieldKey.isEmpty()) {
            throw new BusinessException("fieldKey[" + fieldKey + "]不能为空");
        }
        if (label == null || label.isEmpty()) {
            throw new BusinessException("显示名[" + label + "]不能为空");
        }
        if (valueSource == null || valueSource.isEmpty()) {
            throw new BusinessException("取值来源[" + valueSource + "]不能为空");
        }
        if (sortOrder<1){
            throw new BusinessException("排序不能小于1");
        }
    }
}