package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDetailDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentOptionDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;

@Service
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, Treatment> implements ITreatmentService {

    @Override
    public PageDTO<TreatmentSelectDTO> listTreatmentOptions(TreatmentSelectQuery query) {
        return null;
    }

    @Override
    public PageDTO<TreatmentDTO> queryTreatmentList(TreatmentQuery query) {
        return null;
    }

    @Override
    public TreatmentDetailDTO getTreatmentById(String id) {
        return null;
    }

    @Override
    public List<TreatmentOptionDTO> listCategoryOptions() {
        return Collections.emptyList();
    }
}
