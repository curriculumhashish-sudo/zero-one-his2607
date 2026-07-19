package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import org.springframework.web.bind.annotation.RestController;

import jakarta.annotation.Resource;

/**
 * <p>
 * 描述：诊疗管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
public class TreatmentController implements TreatmentApis {

    @Resource
    private ITreatmentService treatmentService;

    @Override
    public TreatmentResponseDTO addTreatment(TreatmentDTO treatmentDTO) {
        return treatmentService.addTreatment(treatmentDTO);
    }

    @Override
    public TreatmentResponseDTO updateTreatment(TreatmentDTO treatmentDTO) {
        return treatmentService.updateTreatment(treatmentDTO);
    }

    @Override
    public Boolean deleteTreatment(String id) {
        return treatmentService.deleteTreatment(id);
    }

    @Override
    public TreatmentResponseDTO updateStatus(TreatmentStatusDTO statusDTO) {
        return treatmentService.updateStatus(statusDTO);
    }
}