package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.List;

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

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentSelectQuery query) {
        return null;
    }

    @Override
    public PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query) {
        return null;
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        Treatment treatment = this.getById(id);
        if (treatment == null) {
            return null;
        }
        return msTreatmentMapper.toDetailVO(treatment);
    }

    @Override
    public List<String> listCategoryOptions() {
        return TreatmentConstants.listCategoryOptions();
    }

    @Override
    public void addTreatment(TreatmentAddDTO treatmentAddDTO) {
        Treatment treatment = msTreatmentMapper.addDtoToEntity(treatmentAddDTO);
        this.save(treatment);
    }

    @Override
    public TreatmentDetailVO modifyTreatment(TreatmentEditDTO dto) {
        Treatment treatment = this.getById(dto.getId());
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        Treatment updateTreatment = msTreatmentMapper.editDtoToEntity(dto);
        updateTreatment.setCreatedAt(treatment.getCreatedAt());
        updateTreatment.setCreatedBy(treatment.getCreatedBy());
        updateTreatment.setIsDeleted(treatment.getIsDeleted());
        updateTreatment.setDeletedAt(treatment.getDeletedAt());
        updateTreatment.setDeletedBy(treatment.getDeletedBy());
        updateTreatment.setVersion(treatment.getVersion());
        this.updateById(updateTreatment);
        return msTreatmentMapper.toDetailVO(updateTreatment);
    }

    @Override
    public TreatmentDetailVO updateTreatment(TreatmentUpdateDTO dto) {
        Treatment treatment = this.getById(dto.getId());
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        Treatment updateTreatment = msTreatmentMapper.updateDtoToEntity(dto);
        updateTreatment.setCreatedAt(treatment.getCreatedAt());
        updateTreatment.setCreatedBy(treatment.getCreatedBy());
        updateTreatment.setIsDeleted(treatment.getIsDeleted());
        updateTreatment.setDeletedAt(treatment.getDeletedAt());
        updateTreatment.setDeletedBy(treatment.getDeletedBy());
        updateTreatment.setVersion(treatment.getVersion());
        this.updateById(updateTreatment);
        return msTreatmentMapper.toDetailVO(updateTreatment);
    }

    @Override
    public TreatmentDetailVO modifyStatus(TreatmentStatusDTO dto) {
        Treatment treatment = this.getById(dto.getId());
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        treatment.setStatusEnum(dto.getStatusEnum());
        this.updateById(treatment);
        return msTreatmentMapper.toDetailVO(treatment);
    }

    @Override
    public void removeTreatment(String id) {
        Treatment treatment = this.getById(id);
        if (treatment == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        this.removeById(id);
    }
}
