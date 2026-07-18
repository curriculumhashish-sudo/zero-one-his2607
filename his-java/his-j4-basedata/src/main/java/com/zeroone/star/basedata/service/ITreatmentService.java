package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.project.dto.j4.treatment.CreateTreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.ToggleStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.UpdateTreatmentDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentVO;

public interface ITreatmentService extends IService<Treatment> {

    TreatmentVO createTreatment(CreateTreatmentDTO dto);

    TreatmentVO updateTreatment(UpdateTreatmentDTO dto);

    boolean deleteTreatment(String id);

    TreatmentVO toggleStatus(ToggleStatusDTO dto);
}