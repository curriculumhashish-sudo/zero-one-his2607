package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDetailDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentOptionDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;

import java.util.List;

public interface ITreatmentService extends IService<Treatment> {

    PageDTO<TreatmentSelectDTO> listTreatmentOptions(TreatmentSelectQuery query);

    PageDTO<TreatmentDTO> queryTreatmentList(TreatmentQuery query);

    TreatmentDetailDTO getTreatmentById(String id);

    List<TreatmentOptionDTO> listCategoryOptions();
}
