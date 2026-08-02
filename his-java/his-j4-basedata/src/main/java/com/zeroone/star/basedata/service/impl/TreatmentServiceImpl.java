package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.mapper.WkfActivityDefinitionChildMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.components.user.UserHolder;
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

    @Resource
    private UserHolder userHolder;

    /**
     * 获取当前租户ID
     * <p>数据来源：登录用户绑定的租户信息（Redis）</p>
     */
    private String getCurrentTenantId() {
        String tenantId = userHolder.getCurrentTenantId();
        return tenantId != null ? tenantId : "1";
    }

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentQuery query) {
        long pageIndex = (query.getPageIndex() <= 0) ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = (query.getPageSize() <= 0) ? DEFAULT_PAGE_SIZE : query.getPageSize();

        String tenantId = getCurrentTenantId();
        Page<WkfActivityDefinitionDO> page = new Page<>(pageIndex, pageSize);
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        // 只查询选择VO需要的字段，避免select *
        wrapper.select(WkfActivityDefinitionDO::getId, WkfActivityDefinitionDO::getBusNo,
                WkfActivityDefinitionDO::getName, WkfActivityDefinitionDO::getCategoryCode,
                WkfActivityDefinitionDO::getTypeEnum);
        wrapper.eq(WkfActivityDefinitionDO::getTenantId, tenantId);
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
        long pageIndex = query.getPageIndex() <= 0 ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = query.getPageSize() <= 0 ? DEFAULT_PAGE_SIZE : query.getPageSize();

        String tenantId = getCurrentTenantId();
        Page<TreatmentListVO> page = new Page<>(pageIndex, pageSize);
        Page<TreatmentListVO> result = baseMapper.selectTreatmentListPage(page, query, tenantId);
        return PageDTO.create(result);
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        String tenantId = getCurrentTenantId();

        // 主键查询加租户条件，只查询详情VO需要的字段，避免select *
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WkfActivityDefinitionDO::getId, WkfActivityDefinitionDO::getBusNo,
                        WkfActivityDefinitionDO::getName, WkfActivityDefinitionDO::getCategoryCode,
                        WkfActivityDefinitionDO::getTypeEnum, WkfActivityDefinitionDO::getPackageUnitCode,
                        WkfActivityDefinitionDO::getPyStr, WkfActivityDefinitionDO::getWbStr,
                        WkfActivityDefinitionDO::getDescriptionText, WkfActivityDefinitionDO::getYbFlag,
                        WkfActivityDefinitionDO::getYbNo, WkfActivityDefinitionDO::getYbMatchFlag,
                        WkfActivityDefinitionDO::getChrgitmLv, WkfActivityDefinitionDO::getStatusEnum)
                .eq(WkfActivityDefinitionDO::getId, id)
                .eq(WkfActivityDefinitionDO::getTenantId, tenantId);
        WkfActivityDefinitionDO entity = this.getOne(wrapper);
        if (entity == null) {
            throw new RuntimeException("诊疗项目不存在");
        }

        TreatmentDetailVO detailVO = msTreatmentMapper.toDetailVO(entity);

        List<AdmChargeItemDefinitionDO> priceLines = loadPriceLines(id, tenantId);
        enrichPriceFields(detailVO, priceLines);

        List<WkfActivityDefinitionChildDO> children = loadChildren(id, tenantId);
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
        // 新增时写入租户ID，与查询侧按租户过滤保持一致
        wkfActivityDefinitionDO.setTenantId(getCurrentTenantId());
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

    private List<AdmChargeItemDefinitionDO> loadPriceLines(String instanceId, String tenantId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        // 只查询价格回填需要的4个字段，避免select *
        wrapper.select(AdmChargeItemDefinitionDO::getTypeCode, AdmChargeItemDefinitionDO::getYbType,
                AdmChargeItemDefinitionDO::getPriceTypeCode, AdmChargeItemDefinitionDO::getPriceValue);
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, INSTANCE_TABLE)
                .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId)
                .eq(AdmChargeItemDefinitionDO::getIsDeleted, false)
                .eq(AdmChargeItemDefinitionDO::getTenantId, tenantId);
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

    private List<WkfActivityDefinitionChildDO> loadChildren(String activityId, String tenantId) {
        LambdaQueryWrapper<WkfActivityDefinitionChildDO> wrapper = new LambdaQueryWrapper<>();
        // 只查询子项VO需要的5个字段，避免select *
        wrapper.select(WkfActivityDefinitionChildDO::getId, WkfActivityDefinitionChildDO::getActivityId,
                WkfActivityDefinitionChildDO::getChildrenId, WkfActivityDefinitionChildDO::getChildrenNum,
                WkfActivityDefinitionChildDO::getChildrenName);
        wrapper.eq(WkfActivityDefinitionChildDO::getActivityId, activityId)
                .eq(WkfActivityDefinitionChildDO::getTenantId, tenantId);
        List<WkfActivityDefinitionChildDO> children = childMapper.selectList(wrapper);
        return children != null ? children : Collections.emptyList();
    }
}
