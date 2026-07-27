package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.medicine.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.mapper.WkfActivityDefinitionChildMapper;
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
import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Collections;
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

    private static final String INSTANCE_TABLE = "wkf_activity_definition";

    private static final String STATUS_ACTIVE = "ACTIVE";

    private static final String PRICE_TYPE_PURCHASE = "PURCHASE";
    private static final String PRICE_TYPE_SALE = "SALE";
    private static final String PRICE_TYPE_LIMIT = "LIMIT";

    private static final long DEFAULT_PAGE_INDEX = 1L;
    private static final long DEFAULT_PAGE_SIZE = 10L;

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Resource
    private AdmChargeItemDefinitionMapper chargeItemMapper;

    @Resource
    private WkfActivityDefinitionChildMapper childMapper;

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentQuery query) {
        long pageIndex = query.getPageIndex() == null || query.getPageIndex() <= 0 ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = query.getPageSize() == null || query.getPageSize() <= 0 ? DEFAULT_PAGE_SIZE : query.getPageSize();

        Page<WkfActivityDefinitionDO> page = new Page<>(pageIndex, pageSize);
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfActivityDefinitionDO::getStatusEnum, STATUS_ACTIVE);
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(WkfActivityDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getBusNo, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getPyStr, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getWbStr, query.getKeyword())
            );
        }
        if (StrUtil.isNotBlank(query.getCategoryCode())) {
            wrapper.eq(WkfActivityDefinitionDO::getCategoryCode, query.getCategoryCode());
        }
        wrapper.orderByDesc(WkfActivityDefinitionDO::getCreatedAt);
        Page<WkfActivityDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toSelectVO);
    }

    @Override
    public PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query) {
        long pageIndex = query.getPageIndex() == null || query.getPageIndex() <= 0 ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = query.getPageSize() == null || query.getPageSize() <= 0 ? DEFAULT_PAGE_SIZE : query.getPageSize();

        Page<TreatmentListVO> page = new Page<>(pageIndex, pageSize);
        Page<TreatmentListVO> result = baseMapper.selectTreatmentListPage(page, query);
        return PageDTO.create(result);
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        WkfActivityDefinitionDO entity = this.getById(id);
        if (entity == null) {
            throw new RuntimeException("诊疗项目不存在");
        }

        TreatmentDetailVO detailVO = msTreatmentMapper.toDetailVO(entity);

        List<AdmChargeItemDefinitionDO> priceLines = loadPriceLines(id);
        enrichPriceFields(detailVO, priceLines);

        List<WkfActivityDefinitionChildDO> children = loadChildren(id);
        List<WkfActivityDefinitionChildVO> childVOs = msTreatmentMapper.toChildVOList(children);
        detailVO.setChildren(childVOs);

        return detailVO;
    }

    @Override
    public List<String> listCategoryOptions() {
        return TreatmentConstants.listCategoryOptions();
    }

    @Override
    public void addTreatment(TreatmentAddDTO treatmentAddDTO) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = msTreatmentMapper.addDtoToEntity(treatmentAddDTO);
        if (wkfActivityDefinitionDO.getTypeEnum() == null) {
            wkfActivityDefinitionDO.setTypeEnum("1");
        }
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

    private List<AdmChargeItemDefinitionDO> loadPriceLines(String instanceId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, INSTANCE_TABLE)
                .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId)
                .eq(AdmChargeItemDefinitionDO::getIsDeleted, false);
        return chargeItemMapper.selectList(wrapper);
    }

    private void enrichPriceFields(TreatmentDetailVO dto, List<AdmChargeItemDefinitionDO> rows) {
        if (dto == null || rows == null || rows.isEmpty()) {
            return;
        }
        for (AdmChargeItemDefinitionDO row : rows) {
            if (row == null) {
                continue;
            }
            if (dto.getItemTypeCode() == null && row.getTypeCode() != null) {
                dto.setItemTypeCode(row.getTypeCode());
            }
            if (dto.getYbType() == null && row.getYbType() != null) {
                dto.setYbType(row.getYbType());
            }
            if (PRICE_TYPE_PURCHASE.equals(row.getPriceTypeCode())) {
                dto.setPurchasePrice(row.getPriceValue());
                continue;
            }
            if (PRICE_TYPE_LIMIT.equals(row.getPriceTypeCode())) {
                dto.setMaximumRetailPrice(row.getPriceValue());
                continue;
            }
            if (PRICE_TYPE_SALE.equals(row.getPriceTypeCode())) {
                dto.setRetailPrice(row.getPriceValue());
            }
        }
    }

    private List<WkfActivityDefinitionChildDO> loadChildren(String activityId) {
        LambdaQueryWrapper<WkfActivityDefinitionChildDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfActivityDefinitionChildDO::getActivityId, activityId);
        List<WkfActivityDefinitionChildDO> children = childMapper.selectList(wrapper);
        return children != null ? children : Collections.emptyList();
    }
}
