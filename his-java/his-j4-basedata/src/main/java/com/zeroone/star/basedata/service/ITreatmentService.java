package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;

import java.util.List;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;

/**
 * <p>
 * 描述：诊疗管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface ITreatmentService extends IService<Treatment> {

    PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentSelectQuery query);

    PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query);

    TreatmentDetailVO getTreatmentById(String id);

    List<String> listCategoryOptions();


    TreatmentResponseDTO addTreatment(TreatmentDTO treatmentDTO);

    TreatmentResponseDTO updateTreatment(TreatmentDTO treatmentDTO);

    Boolean deleteTreatment(String id);

    TreatmentResponseDTO updateStatus(TreatmentStatusDTO statusDTO);
}