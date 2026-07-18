package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.j4.basedata.dto.TreatmentDTO;
import com.zeroone.star.project.j4.basedata.dto.TreatmentResponseDTO;
import com.zeroone.star.project.j4.basedata.dto.TreatmentStatusDTO;
import org.springframework.beans.BeanUtils;
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
        Treatment treatment = new Treatment();
        BeanUtils.copyProperties(treatmentDTO, treatment);
        treatment.setCreatedAt(LocalDateTime.now());
        treatment.setIsDeleted(0);
        treatment.setVersion(0);
        this.save(treatment);
        return convertToResponseDTO(treatment);
    }

    @Override
    public TreatmentResponseDTO updateTreatment(TreatmentDTO treatmentDTO) {
        Treatment treatment = this.getById(treatmentDTO.getId());
        if (treatment != null) {
            BeanUtils.copyProperties(treatmentDTO, treatment);
            treatment.setUpdatedAt(LocalDateTime.now());
            this.updateById(treatment);
        }
        return convertToResponseDTO(treatment);
    }

    @Override
    public Boolean deleteTreatment(String id) {
        Treatment treatment = this.getById(id);
        if (treatment != null) {
            treatment.setIsDeleted(1);
            treatment.setDeletedAt(LocalDateTime.now());
            this.updateById(treatment);
            return true;
        }
        return false;
    }

    @Override
    public TreatmentResponseDTO updateStatus(TreatmentStatusDTO statusDTO) {
        Treatment treatment = this.getById(statusDTO.getId());
        if (treatment != null) {
            treatment.setStatusEnum(statusDTO.getStatusEnum());
            treatment.setUpdatedAt(LocalDateTime.now());
            this.updateById(treatment);
        }
        return convertToResponseDTO(treatment);
    }

    private TreatmentResponseDTO convertToResponseDTO(Treatment treatment) {
        TreatmentResponseDTO responseDTO = new TreatmentResponseDTO();
        if (treatment != null) {
            BeanUtils.copyProperties(treatment, responseDTO);
        }
        return responseDTO;
    }
}