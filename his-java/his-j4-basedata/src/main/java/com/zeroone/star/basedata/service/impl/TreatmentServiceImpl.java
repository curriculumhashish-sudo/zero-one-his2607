package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.CreateTreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.ToggleStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.UpdateTreatmentDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentVO;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.UUID;

@Service
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, Treatment> implements ITreatmentService {

    @Override
    public TreatmentVO createTreatment(CreateTreatmentDTO dto) {
        Treatment treatment = new Treatment();
        BeanUtils.copyProperties(dto, treatment);
        treatment.setId(UUID.randomUUID().toString());
        treatment.setCreatedAt(LocalDateTime.now());
        treatment.setIsDeleted(0);
        treatment.setVersion(0);
        baseMapper.insert(treatment);
        return convertToVO(treatment);
    }

    @Override
    public TreatmentVO updateTreatment(UpdateTreatmentDTO dto) {
        Treatment treatment = baseMapper.selectById(dto.getId());
        if (treatment == null) {
            return null;
        }
        BeanUtils.copyProperties(dto, treatment);
        treatment.setUpdatedAt(LocalDateTime.now());
        baseMapper.updateById(treatment);
        return convertToVO(treatment);
    }

    @Override
    public boolean deleteTreatment(String id) {
        Treatment treatment = baseMapper.selectById(id);
        if (treatment == null) {
            return false;
        }
        treatment.setIsDeleted(1);
        treatment.setDeletedAt(LocalDateTime.now());
        baseMapper.updateById(treatment);
        return true;
    }

    @Override
    public TreatmentVO toggleStatus(ToggleStatusDTO dto) {
        Treatment treatment = baseMapper.selectById(dto.getId());
        if (treatment == null) {
            return null;
        }
        treatment.setStatusEnum(dto.getStatusEnum());
        treatment.setUpdatedAt(LocalDateTime.now());
        baseMapper.updateById(treatment);
        return convertToVO(treatment);
    }

    private TreatmentVO convertToVO(Treatment treatment) {
        TreatmentVO vo = new TreatmentVO();
        BeanUtils.copyProperties(treatment, vo);
        return vo;
    }
}