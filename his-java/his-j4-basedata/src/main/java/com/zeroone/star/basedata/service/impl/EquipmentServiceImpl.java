package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.equipment.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.basedata.mapper.EquipmentChargeItemMapper;
import com.zeroone.star.basedata.mapper.EquipmentMapper;
import com.zeroone.star.basedata.service.IEquipmentService;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：器材管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Service
public class EquipmentServiceImpl extends ServiceImpl<EquipmentMapper, AdmDeviceDefinitionDO> implements IEquipmentService {

    @Resource
    private MsEquipmentMapper msEquipmentMapper;
    @Resource
    private EquipmentChargeItemMapper chargeItemMapper;

    /**
     * 获取器材列表（条件+分页）
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 器材信息分页列表
     */
    @Override
    public Page<AdmDeviceDefinitionDO> queryEquipmentPage(EquipmentPageQuery query) {
        // 1.构建查询条件
        LambdaQueryWrapper<AdmDeviceDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getKeyword())) {
            // 关键词查询
            wrapper.and(w -> w.like(AdmDeviceDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(AdmDeviceDefinitionDO::getBusNo, query.getKeyword()));
        }

        // 2.分类筛选
        if (StringUtils.hasText(query.getCategoryCode())) {
            wrapper.eq(AdmDeviceDefinitionDO::getCategoryCode, query.getCategoryCode());
        }

        // 3.排序
        wrapper.orderByAsc(AdmDeviceDefinitionDO::getBusNo);

        // 4.构建分页对象
        Page<AdmDeviceDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // 5.执行分页查询
        return baseMapper.selectPage(page, wrapper);
    }

    /**
     * 修改器材
     *
     * @param dto 修改器材的DTO
     * @return 修改后的器材实体
     * @throws IllegalArgumentException 器材不存在或参数非法
     */
    @Override
    public AdmDeviceDefinitionDO updateEquipment(EquipmentUpdateDTO dto) {
        String id = dto.getId();
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("修改器材时主键不可为空");
        }

        AdmDeviceDefinitionDO existing = baseMapper.selectById(Long.valueOf(id));
        if (existing == null) {
            throw new IllegalArgumentException("器材不存在，主键：" + id);
        }
        // ms转换
        AdmDeviceDefinitionDO dtoEntity = msEquipmentMapper.updateDtoToEntity(dto);
        // 复制属性
        cn.hutool.core.bean.BeanUtil.copyProperties(dtoEntity, existing,
                cn.hutool.core.bean.copier.CopyOptions.create().ignoreNullValue());
        baseMapper.updateById(existing);

        // 同步价格表
        Long deviceId = Long.valueOf(id);
        saveOrUpdatePrice(deviceId, "PURCHASE", dto.getPurchasePrice(), "采购价", dto.getItemTypeCode(), dto.getOrgId(), dto.getYbType());
        saveOrUpdatePrice(deviceId, "SALE", dto.getRetailPrice(), "零售价", dto.getItemTypeCode(), dto.getOrgId(), dto.getYbType());
        saveOrUpdatePrice(deviceId, "LIMIT", dto.getMaximumRetailPrice(), "限价", dto.getItemTypeCode(), dto.getOrgId(), dto.getYbType());

        return existing;
    }

    @Override
    public BigDecimal getRetailPrice(Long deviceId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, "adm_device_definition")
               .eq(AdmChargeItemDefinitionDO::getInstanceId, deviceId)
               .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, "SALE")
               .last("LIMIT 1");
        AdmChargeItemDefinitionDO priceRecord = chargeItemMapper.selectOne(wrapper);
        return priceRecord != null ? priceRecord.getPriceValue() : null;
    }

    private void saveOrUpdatePrice(Long instanceId, String priceTypeCode, BigDecimal priceValue,
                                    String chargeName, String typeCode, Long orgId, String ybType) {
        if (priceValue == null) {
            return;
        }
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, "adm_device_definition")
               .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId)
               .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, priceTypeCode)
               .last("LIMIT 1");
        AdmChargeItemDefinitionDO existingPrice = chargeItemMapper.selectOne(wrapper);

        if (existingPrice != null) {
            existingPrice.setPriceValue(priceValue);
            existingPrice.setUpdatedAt(LocalDateTime.now());
            chargeItemMapper.updateById(existingPrice);
        } else {
            AdmChargeItemDefinitionDO newPrice = new AdmChargeItemDefinitionDO();
            newPrice.setInstanceTable("adm_device_definition");
            newPrice.setInstanceId(instanceId);
            newPrice.setPriceTypeCode(priceTypeCode);
            newPrice.setPriceValue(priceValue);
            newPrice.setChargeName(chargeName);
            newPrice.setStatusEnum("ACTIVE");
            newPrice.setTypeCode(typeCode);
            newPrice.setOrgId(orgId);
            newPrice.setYbType(ybType);
            newPrice.setCreatedAt(LocalDateTime.now());
            newPrice.setUpdatedAt(LocalDateTime.now());
            chargeItemMapper.insert(newPrice);
        }
    }

    /**
     * 启/停用器材（支持批量）
     *
     * @param dto 切换状态请求（主键列表）
     * @return 成功修改的数量
     * @throws IllegalArgumentException 器材不存在或参数非法
     */
    @Override
    public int modifyEquipmentStatus(EquipmentStatusDTO dto) {
        int count = 0;
        for (String id : dto.getIds()) {
            AdmDeviceDefinitionDO existing = baseMapper.selectById(Long.valueOf(id));
            if (existing == null) {
                throw new IllegalArgumentException("器材不存在，主键：" + id);
            }
            boolean newStatus = existing.getActiveFlag() == null || !existing.getActiveFlag();
            existing.setActiveFlag(newStatus);
            baseMapper.updateById(existing);
            count++;
        }
        return count;
    }
}
