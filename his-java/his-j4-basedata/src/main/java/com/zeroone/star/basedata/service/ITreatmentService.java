package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;

import java.util.List;

/**
 * <p>
 * 描述：诊疗管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface ITreatmentService extends IService<WkfActivityDefinitionDO> {

    PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentQuery query);

    PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query);

    TreatmentDetailVO getTreatmentById(String id);

    List<String> listCategoryOptions();

    void addTreatment(TreatmentAddDTO treatmentAddDTO);

    TreatmentDetailVO modifyTreatment(TreatmentEditDTO dto);

    TreatmentDetailVO updateTreatment(TreatmentUpdateDTO dto);

    TreatmentDetailVO modifyStatus(TreatmentStatusDTO dto);

    void removeTreatment(String id);
}
