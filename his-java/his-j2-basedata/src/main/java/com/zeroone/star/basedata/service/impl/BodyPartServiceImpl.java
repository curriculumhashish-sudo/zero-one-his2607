package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.BodyPart;
import com.zeroone.star.basedata.mapper.BodyPartMapper;
import com.zeroone.star.basedata.service.BodyPartBusinessException;
import com.zeroone.star.basedata.service.BodyPartService;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartStatusDTO;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.HashSet;
import java.util.Set;

/**
 * 部位管理业务实现。
 */
@Service
public class BodyPartServiceImpl extends ServiceImpl<BodyPartMapper, BodyPart> implements BodyPartService {

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void modifyBodyPart(BodyPartModifyDTO dto) {
        requireBodyPart(dto.getId());
        String parentId = normalizeParentId(dto.getParentId());
        validateParent(dto.getId(), parentId);
        validateSameLevelName(dto.getId(), dto.getPartName().trim(), parentId);

        int affectedRows = baseMapper.update(null, new LambdaUpdateWrapper<BodyPart>()
                .eq(BodyPart::getId, dto.getId())
                .eq(BodyPart::getVersion, dto.getVersion())
                .set(BodyPart::getName, dto.getPartName().trim())
                .set(BodyPart::getParentId, parentId)
                .set(BodyPart::getPyStr, dto.getPyStr())
                .set(BodyPart::getWbStr, dto.getWbStr())
                .set(BodyPart::getVersion, dto.getVersion() + 1)
                .set(BodyPart::getUpdatedAt, LocalDateTime.now()));
        assertVersionUpdated(affectedRows);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeBodyPart(String id) {
        requireBodyPart(id);
        if (count(new LambdaQueryWrapper<BodyPart>().eq(BodyPart::getParentId, id)) > 0) {
            throw new BodyPartBusinessException("该部位存在未删除的下级部位，不能删除");
        }
        if (baseMapper.existsClinicalReference(id)) {
            throw new BodyPartBusinessException("该部位已被临床业务使用，不能删除");
        }

        int affectedRows = baseMapper.update(null, new LambdaUpdateWrapper<BodyPart>()
                .eq(BodyPart::getId, id)
                .set(BodyPart::getIsDeleted, 1)
                .set(BodyPart::getDeletedAt, LocalDateTime.now())
                .set(BodyPart::getUpdatedAt, LocalDateTime.now()));
        if (affectedRows != 1) {
            throw new BodyPartBusinessException("部位已被删除，请刷新后重试");
        }
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void modifyBodyPartStatus(BodyPartStatusDTO dto) {
        BodyPart bodyPart = requireBodyPart(dto.getId());
        if (dto.getStatusEnum().equals(bodyPart.getStatusEnum())) {
            return;
        }

        int affectedRows = baseMapper.update(null, new LambdaUpdateWrapper<BodyPart>()
                .eq(BodyPart::getId, dto.getId())
                .eq(BodyPart::getVersion, dto.getVersion())
                .set(BodyPart::getStatusEnum, dto.getStatusEnum())
                .set(BodyPart::getVersion, dto.getVersion() + 1)
                .set(BodyPart::getUpdatedAt, LocalDateTime.now()));
        assertVersionUpdated(affectedRows);
    }

    private BodyPart requireBodyPart(String id) {
        BodyPart bodyPart = getById(id);
        if (bodyPart == null) {
            throw new BodyPartBusinessException("部位不存在或已删除");
        }
        return bodyPart;
    }

    private void validateParent(String id, String parentId) {
        if (!StringUtils.hasText(parentId)) {
            return;
        }
        if (id.equals(parentId)) {
            throw new BodyPartBusinessException("上级部位不能是自身");
        }

        Set<String> visitedIds = new HashSet<>();
        String currentId = parentId;
        while (StringUtils.hasText(currentId)) {
            if (!visitedIds.add(currentId)) {
                throw new BodyPartBusinessException("部位层级数据异常，存在循环引用");
            }
            if (id.equals(currentId)) {
                throw new BodyPartBusinessException("上级部位不能是当前部位的下级节点");
            }
            BodyPart parent = getById(currentId);
            if (parent == null) {
                throw new BodyPartBusinessException("上级部位不存在或已删除");
            }
            currentId = parent.getParentId();
        }
    }

    private void validateSameLevelName(String id, String name, String parentId) {
        LambdaQueryWrapper<BodyPart> query = new LambdaQueryWrapper<BodyPart>()
                .eq(BodyPart::getName, name)
                .ne(BodyPart::getId, id);
        if (parentId == null) {
            query.isNull(BodyPart::getParentId);
        } else {
            query.eq(BodyPart::getParentId, parentId);
        }
        if (count(query) > 0) {
            throw new BodyPartBusinessException("同一上级部位下已存在相同名称");
        }
    }

    private String normalizeParentId(String parentId) {
        return StringUtils.hasText(parentId) ? parentId.trim() : null;
    }

    private void assertVersionUpdated(int affectedRows) {
        if (affectedRows != 1) {
            throw new BodyPartBusinessException("部位数据已被其他用户修改，请刷新后重试");
        }
    }
}
