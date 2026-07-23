package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
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
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, WkfActivityDefinitionDO> implements ITreatmentService {

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentSelectQuery query) {
        Page<WkfActivityDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(WkfActivityDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getBusNo, query.getKeyword())
            );
        }
        wrapper.orderByAsc(WkfActivityDefinitionDO::getCreatedAt);
        Page<WkfActivityDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toSelectVO);
    }

    @Override
    public PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query) {
        Page<WkfActivityDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(WkfActivityDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getBusNo, query.getKeyword())
            );
        }
        if (StrUtil.isNotBlank(query.getCategoryCode())) {
            wrapper.eq(WkfActivityDefinitionDO::getCategoryCode, query.getCategoryCode());
        }
        wrapper.orderByAsc(WkfActivityDefinitionDO::getCreatedAt);
        Page<WkfActivityDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toListVO);
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(id);
        if (wkfActivityDefinitionDO == null) {
            return null;
        }
        return msTreatmentMapper.toDetailVO(wkfActivityDefinitionDO);
    }

    @Override
    public List<String> listCategoryOptions() {
        return TreatmentConstants.listCategoryOptions();
    }

    @Override
    public void addTreatment(TreatmentAddDTO treatmentAddDTO) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = msTreatmentMapper.addDtoToEntity(treatmentAddDTO);
        this.save(wkfActivityDefinitionDO);
    }

    @Override
    public TreatmentDetailVO modifyTreatment(TreatmentEditDTO dto) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(dto.getId());
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        WkfActivityDefinitionDO updateWkfActivityDefinitionDO = msTreatmentMapper.editDtoToEntity(dto);
        updateWkfActivityDefinitionDO.setCreatedAt(wkfActivityDefinitionDO.getCreatedAt());
        updateWkfActivityDefinitionDO.setCreatedBy(wkfActivityDefinitionDO.getCreatedBy());
        updateWkfActivityDefinitionDO.setIsDeleted(wkfActivityDefinitionDO.getIsDeleted());
        updateWkfActivityDefinitionDO.setDeletedAt(wkfActivityDefinitionDO.getDeletedAt());
        updateWkfActivityDefinitionDO.setDeletedBy(wkfActivityDefinitionDO.getDeletedBy());
        updateWkfActivityDefinitionDO.setVersion(wkfActivityDefinitionDO.getVersion());
        this.updateById(updateWkfActivityDefinitionDO);
        return msTreatmentMapper.toDetailVO(updateWkfActivityDefinitionDO);
    }

    @Override
    public TreatmentDetailVO updateTreatment(TreatmentUpdateDTO dto) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(dto.getId());
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        WkfActivityDefinitionDO updateWkfActivityDefinitionDO = msTreatmentMapper.updateDtoToEntity(dto);
        updateWkfActivityDefinitionDO.setCreatedAt(wkfActivityDefinitionDO.getCreatedAt());
        updateWkfActivityDefinitionDO.setCreatedBy(wkfActivityDefinitionDO.getCreatedBy());
        updateWkfActivityDefinitionDO.setIsDeleted(wkfActivityDefinitionDO.getIsDeleted());
        updateWkfActivityDefinitionDO.setDeletedAt(wkfActivityDefinitionDO.getDeletedAt());
        updateWkfActivityDefinitionDO.setDeletedBy(wkfActivityDefinitionDO.getDeletedBy());
        updateWkfActivityDefinitionDO.setVersion(wkfActivityDefinitionDO.getVersion());
        this.updateById(updateWkfActivityDefinitionDO);
        return msTreatmentMapper.toDetailVO(updateWkfActivityDefinitionDO);
    }

    @Override
    public TreatmentDetailVO modifyStatus(TreatmentStatusDTO dto) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(dto.getId());
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        wkfActivityDefinitionDO.setStatusEnum(dto.getStatusEnum());
        this.updateById(wkfActivityDefinitionDO);
        return msTreatmentMapper.toDetailVO(wkfActivityDefinitionDO);
    }

    @Override
    public void removeTreatment(String id) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(id);
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        this.removeById(id);
    }
}
