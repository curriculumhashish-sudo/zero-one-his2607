package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.TreatmentConstants;
import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.WkfActivityDefinitionMapper;
import com.zeroone.star.basedata.service.IServiceCatalog;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogAddDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogEditDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogUpdateDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogStatusDTO;
import com.zeroone.star.project.query.j4.treatment.ServiceCatalogQuery;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogDetailVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogListVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogSelectVO;
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
public class ServiceCatalogImpl extends ServiceImpl<WkfActivityDefinitionMapper, WkfActivityDefinitionDO> implements IServiceCatalog {

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Override
    public PageDTO<ServiceCatalogSelectVO> listServiceCatalogOptions(ServiceCatalogQuery query) {
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
    public PageDTO<ServiceCatalogListVO> queryServiceCatalogList(ServiceCatalogQuery query) {
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
    public ServiceCatalogDetailVO getServiceCatalogById(String id) {
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
    public void addServiceCatalog(ServiceCatalogAddDTO serviceCatalogAddDTO) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = msTreatmentMapper.addDtoToEntity(serviceCatalogAddDTO);
        this.save(wkfActivityDefinitionDO);
    }

    @Override
    public ServiceCatalogDetailVO modifyServiceCatalog(ServiceCatalogEditDTO dto) {
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
    public ServiceCatalogDetailVO updateServiceCatalog(ServiceCatalogUpdateDTO dto) {
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
    public ServiceCatalogDetailVO modifyStatus(ServiceCatalogStatusDTO dto) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(dto.getId());
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        wkfActivityDefinitionDO.setStatusEnum(dto.getStatusEnum());
        this.updateById(wkfActivityDefinitionDO);
        return msTreatmentMapper.toDetailVO(wkfActivityDefinitionDO);
    }

    @Override
    public void removeServiceCatalog(String id) {
        WkfActivityDefinitionDO wkfActivityDefinitionDO = this.getById(id);
        if (wkfActivityDefinitionDO == null) {
            throw new RuntimeException("诊疗项目不存在");
        }
        this.removeById(id);
    }
}
