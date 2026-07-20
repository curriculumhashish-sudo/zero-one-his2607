package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.TreatmentDO;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
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
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, TreatmentDO> implements ITreatmentService {

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentQuery query) {
        Page<TreatmentDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<TreatmentDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(com.zeroone.star.basedata.entity.TreatmentDO::getName, query.getKeyword())
                    .or()
                    .like(TreatmentDO::getBusNo, query.getKeyword())
            );
        }
        wrapper.orderByAsc(com.zeroone.star.basedata.entity.TreatmentDO::getCreatedAt);
        Page<TreatmentDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toSelectVO);
    }

    @Override
    public PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query) {
        Page<TreatmentDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<TreatmentDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(com.zeroone.star.basedata.entity.TreatmentDO::getName, query.getKeyword())
                    .or()
                    .like(com.zeroone.star.basedata.entity.TreatmentDO::getBusNo, query.getKeyword())
            );
        }
        if (StrUtil.isNotBlank(query.getCategoryCode())) {
            wrapper.eq(com.zeroone.star.basedata.entity.TreatmentDO::getCategoryCode, query.getCategoryCode());
        }
        wrapper.orderByAsc(com.zeroone.star.basedata.entity.TreatmentDO::getCreatedAt);
        Page<TreatmentDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toListVO);
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        TreatmentDO treatmentDO = this.getById(id);
        if (treatmentDO == null) {
            return null;
        }
        return msTreatmentMapper.toDetailVO(treatmentDO);
    }

    @Override
    public List<String> listCategoryOptions() {
        return TreatmentConstants.listCategoryOptions();
    }

    @Override
    public void addTreatment(TreatmentAddDTO treatmentAddDTO) {
        TreatmentDO treatmentDO = msTreatmentMapper.addDtoToEntity(treatmentAddDTO);
        this.save(treatmentDO);
    }

    @Override
    public TreatmentDetailVO modifyTreatment(TreatmentEditDTO dto) {
        TreatmentDO treatmentDO = this.getById(dto.getId());
        if (treatmentDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        TreatmentDO updateTreatmentDO = msTreatmentMapper.editDtoToEntity(dto);
        updateTreatmentDO.setCreatedAt(treatmentDO.getCreatedAt());
        updateTreatmentDO.setCreatedBy(treatmentDO.getCreatedBy());
        updateTreatmentDO.setIsDeleted(treatmentDO.getIsDeleted());
        updateTreatmentDO.setDeletedAt(treatmentDO.getDeletedAt());
        updateTreatmentDO.setDeletedBy(treatmentDO.getDeletedBy());
        updateTreatmentDO.setVersion(treatmentDO.getVersion());
        this.updateById(updateTreatmentDO);
        return msTreatmentMapper.toDetailVO(updateTreatmentDO);
    }

    @Override
    public TreatmentDetailVO updateTreatment(TreatmentUpdateDTO dto) {
        TreatmentDO treatmentDO = this.getById(dto.getId());
        if (treatmentDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        TreatmentDO updateTreatmentDO = msTreatmentMapper.updateDtoToEntity(dto);
        updateTreatmentDO.setCreatedAt(treatmentDO.getCreatedAt());
        updateTreatmentDO.setCreatedBy(treatmentDO.getCreatedBy());
        updateTreatmentDO.setIsDeleted(treatmentDO.getIsDeleted());
        updateTreatmentDO.setDeletedAt(treatmentDO.getDeletedAt());
        updateTreatmentDO.setDeletedBy(treatmentDO.getDeletedBy());
        updateTreatmentDO.setVersion(treatmentDO.getVersion());
        this.updateById(updateTreatmentDO);
        return msTreatmentMapper.toDetailVO(updateTreatmentDO);
    }

    @Override
    public TreatmentDetailVO modifyStatus(TreatmentStatusDTO dto) {
        TreatmentDO treatmentDO = this.getById(dto.getId());
        if (treatmentDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        treatmentDO.setStatusEnum(dto.getStatusEnum());
        this.updateById(treatmentDO);
        return msTreatmentMapper.toDetailVO(treatmentDO);
    }

    @Override
    public void removeTreatment(String id) {
        TreatmentDO treatmentDO = this.getById(id);
        if (treatmentDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        this.removeById(id);
    }
}
