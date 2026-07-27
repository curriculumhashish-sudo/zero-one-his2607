package com.zeroone.star.sysmgr.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.j2.sysmgr.DictItemUpdateDTO;
import com.zeroone.star.sysmgr.entity.TerConcept;
import com.zeroone.star.sysmgr.mapper.TerConceptMapper;
import com.zeroone.star.sysmgr.service.ITerConceptService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;

/**
 * <p>
 * 字典项 服务实现类
 * </p>
 *
 * @author xingluo
 */
@Service
public class TerConceptServiceImpl extends ServiceImpl<TerConceptMapper, TerConcept> 
        implements ITerConceptService {

    @Autowired
    private MSDictItemMapper msDictItemMapper;

    /**
     *
     * 更行编码
     * @param dto 修改参数
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void updateDict(DictItemUpdateDTO dto) {
        // 1. 校验数据是否存在
        TerConcept entity = getById(dto.getId());
        if (entity == null || Boolean.TRUE.equals(entity.getIsDeleted())) {
            throw new RuntimeException("字典项id不存在");
        }

        // 2. DTO字段更新到实体（只改传了的字段，其他保留原值）
        msDictItemMapper.updateDtoToEntity(dto, entity);

        // 3. 设置更新时间
        entity.setUpdatedAt(LocalDateTime.now());

        // 4. 保存
        updateById(entity);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void deleteDict(Long id) {
        // 1. 校验数据是否存在
        TerConcept entity = getById(id);
        if (entity == null || Boolean.TRUE.equals(entity.getIsDeleted())) {
            throw new RuntimeException("字典项不存在");
        }

        // 2. 校验是否被引用：检查有没有子节点
        long childCount = count(new LambdaQueryWrapper<TerConcept>()
                .eq(TerConcept::getParentId, id)
                .eq(TerConcept::getIsDeleted, false)
        );
        if (childCount > 0) {
            throw new RuntimeException("该字典项下存在子项，无法删除");
        }

        // 3. 逻辑删除
        entity.setIsDeleted(true);
        entity.setDeletedAt(LocalDateTime.now());
        updateById(entity);
    }

}
