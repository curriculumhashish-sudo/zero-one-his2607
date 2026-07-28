package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.configuration.entity.DocumentTypeDO;
import com.zeroone.star.configuration.mapper.DocumentTypeMapper;
import com.zeroone.star.configuration.service.DocumentTypeService;
import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

/**
 * 文书类型服务实现类
 */
@Service
@RequiredArgsConstructor
public class DocumentTypeServiceImpl implements DocumentTypeService {
    
    private final DocumentTypeMapper documentTypeMapper;
    
    @Override
    public JsonVO<Void> add(DocumentTypeAddDTO dto) {
        // 检查编码是否已存在
        LambdaQueryWrapper<DocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(DocumentTypeDO::getCode, dto.getCode());
        if (documentTypeMapper.selectCount(wrapper) > 0) {
            throw new RuntimeException("文书类型编码已存在");
        }
        
        // 构建实体
        DocumentTypeDO entity = new DocumentTypeDO();
        BeanUtils.copyProperties(dto, entity);
        entity.setIsEnabled(dto.getIsEnabled() != null ? dto.getIsEnabled() : true);
        
        // 插入数据库
        documentTypeMapper.insert(entity);
        
        return JsonVO.success(null);
    }
    
    @Override
    public JsonVO<Void> update(DocumentTypeUpdateDTO dto) {
        // 检查是否存在
        DocumentTypeDO entity = documentTypeMapper.selectById(dto.getId());
        if (entity == null) {
            throw new RuntimeException("文书类型不存在");
        }
        
        // 如果修改了编码，检查新编码是否已被使用
        if (!entity.getCode().equals(dto.getCode())) {
            LambdaQueryWrapper<DocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
            wrapper.eq(DocumentTypeDO::getCode, dto.getCode())
                   .ne(DocumentTypeDO::getId, dto.getId());
            if (documentTypeMapper.selectCount(wrapper) > 0) {
                throw new RuntimeException("文书类型编码已存在");
            }
        }
        
        // 更新实体
        BeanUtils.copyProperties(dto, entity);
        documentTypeMapper.updateById(entity);
        
        return JsonVO.success(null);
    }
    
    @Override
    public JsonVO<Void> delete(String id) {
        // 检查是否存在
        DocumentTypeDO entity = documentTypeMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("文书类型不存在");
        }
        
        // 软删除
        documentTypeMapper.deleteById(id);
        
        return JsonVO.success(null);
    }
}
