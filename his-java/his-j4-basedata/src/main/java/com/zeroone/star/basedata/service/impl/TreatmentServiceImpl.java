package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.convert.TreatmentConvert;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, Treatment> implements ITreatmentService {

    @Override
    public TreatmentResponseDTO addTreatment(TreatmentDTO treatmentDTO) {
        Treatment treatment = TreatmentConvert.INSTANCE.dtoToEntity(treatmentDTO);
        treatment.setCreatedAt(LocalDateTime.now());
        treatment.setIsDeleted(0);
        treatment.setVersion(0);
        this.save(treatment);
        return TreatmentConvert.INSTANCE.entityToResponseDTO(treatment);
    }

    @Override
    public TreatmentResponseDTO updateTreatment(TreatmentDTO treatmentDTO) {
        Treatment treatment = this.getById(treatmentDTO.getId());
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        Treatment updateTreatment = TreatmentConvert.INSTANCE.dtoToEntity(treatmentDTO);
        updateTreatment.setCreatedAt(treatment.getCreatedAt());
        updateTreatment.setCreatedBy(treatment.getCreatedBy());
        updateTreatment.setIsDeleted(treatment.getIsDeleted());
        updateTreatment.setDeletedAt(treatment.getDeletedAt());
        updateTreatment.setDeletedBy(treatment.getDeletedBy());
        updateTreatment.setVersion(treatment.getVersion());
        updateTreatment.setUpdatedAt(LocalDateTime.now());
        this.updateById(updateTreatment);
        return TreatmentConvert.INSTANCE.entityToResponseDTO(updateTreatment);
    }

    @Override
    public Boolean deleteTreatment(String id) {
        Treatment treatment = this.getById(id);
        if (treatment == null) {
            return false;
        }
        treatment.setIsDeleted(1);
        treatment.setDeletedAt(LocalDateTime.now());
        this.updateById(treatment);
        return true;
    }

    @Override
    public TreatmentResponseDTO updateStatus(TreatmentStatusDTO statusDTO) {
        Treatment treatment = this.getById(statusDTO.getId());
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        treatment.setStatusEnum(statusDTO.getStatusEnum());
        treatment.setUpdatedAt(LocalDateTime.now());
        this.updateById(treatment);
        return TreatmentConvert.INSTANCE.entityToResponseDTO(treatment);
    }
}