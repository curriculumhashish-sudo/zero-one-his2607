package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.EquipmentCategoryConstants;
import com.zeroone.star.basedata.constant.EquipmentConstants;
import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.EquipmentMapper;
import com.zeroone.star.basedata.service.IEquipmentService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import jakarta.annotation.Resource;
import lombok.RequiredArgsConstructor;
import lombok.SneakyThrows;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Objects;

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
@RequiredArgsConstructor
public class EquipmentServiceImpl extends ServiceImpl<EquipmentMapper, AdmDeviceDefinitionDO> implements IEquipmentService {

    private final MsEquipmentMapper msEquipmentMapper;
    private final AdmChargeItemDefinitionMapper chargeItemMapper;

    // UserHoler用于获取租户ID
    @Resource
    private UserHolder userHolder;

    /**
     * 获取器材列表（条件+分页）
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 器材信息分页列表
     */
    @Override
    public PageDTO<EquipmentVO> queryEquipmentPage(EquipmentPageQuery query) {
        // 1.构建查询条件
        LambdaQueryWrapper<AdmDeviceDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        String tenantId = userHolder.getCurrentTenantId(); // 租户ID
        wrapper.eq(AdmDeviceDefinitionDO::getTenantId, tenantId);
        if (StringUtils.hasText(query.getKeyword())) { //关键词
            wrapper.and(w -> w.like(AdmDeviceDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(AdmDeviceDefinitionDO::getBusNo, query.getKeyword()));
        }
        if (StringUtils.hasText(query.getCategoryCode())) { //分类编码
            wrapper.eq(AdmDeviceDefinitionDO::getCategoryCode, query.getCategoryCode());
        }
        wrapper.orderByAsc(AdmDeviceDefinitionDO::getBusNo); //业务编号升序

        // 2.构建分页对象
        Page<AdmDeviceDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // 3.执行分页查询
        Page<AdmDeviceDefinitionDO> entityPage = baseMapper.selectPage(page, wrapper);

        // 4. Entity→VO转换+分类名称填充
        return PageDTO.create(entityPage, entity -> {
            EquipmentVO vo = msEquipmentMapper.entityToVo(entity);
            vo.setCategoryName(EquipmentCategoryConstants.getCategoryName(entity.getCategoryCode()));
            return vo;
        });
    }

    /**
     * 修改器材
     *
     * @param dto 修改器材的DTO
     * @return 修改后的器材实体
     * @throws IllegalArgumentException 器材不存在或参数非法
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public EquipmentVO updateEquipment(EquipmentUpdateDTO dto) {
        String id = dto.getId();
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("修改器材时主键不可为空");
        }
        String currentTenantId = userHolder.getCurrentTenantId();
        AdmDeviceDefinitionDO existing = baseMapper.selectById(id);
        if (existing == null || !currentTenantId.equals(existing.getTenantId())) {
            throw new IllegalArgumentException("器材不存在，主键：" + id);
        }
        // ms转换
        AdmDeviceDefinitionDO dtoEntity = msEquipmentMapper.updateDtoToEntity(dto);
        // 复制属性（ignoreNullValue 保留 version 不被覆盖，乐观锁生效）
        cn.hutool.core.bean.BeanUtil.copyProperties(dtoEntity, existing,
                cn.hutool.core.bean.copier.CopyOptions.create().ignoreNullValue());
        int updated = baseMapper.updateById(existing);
        if (updated == 0) {
            AdmDeviceDefinitionDO latest = baseMapper.selectById(id);
            if (latest == null || !Objects.equals(latest.getVersion(), existing.getVersion())) {
                throw new RuntimeException("数据已被他人修改，请刷新后重试");
            }
        }

        // 同步价格表
        String deviceId = id;
        // 执行机构ID由登录上下文决定，不信任前端传入
        String orgIdStr = userHolder.getCurrentOrgId();
        saveOrUpdatePrice(deviceId, "PURCHASE", dto.getPurchasePrice(), "采购价", dto.getItemTypeCode(), orgIdStr, dto.getYbType());
        saveOrUpdatePrice(deviceId, "SALE", dto.getRetailPrice(), "零售价", dto.getItemTypeCode(), orgIdStr, dto.getYbType());
        saveOrUpdatePrice(deviceId, "LIMIT", dto.getMaximumRetailPrice(), "限价", dto.getItemTypeCode(), orgIdStr, dto.getYbType());

        // Entity→VO 转换 + 零售价填充
        EquipmentVO vo = msEquipmentMapper.entityToVo(existing);
        vo.setRetailPrice(getRetailPrice(id));
        return vo;
    }

    // 获取零售价格
    private BigDecimal getRetailPrice(String deviceId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, "adm_device_definition")
                .eq(AdmChargeItemDefinitionDO::getInstanceId, deviceId)
                .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, "SALE")
                .last("LIMIT 1");
        AdmChargeItemDefinitionDO priceRecord = chargeItemMapper.selectOne(wrapper);
        return priceRecord != null ? priceRecord.getPriceValue() : null;
    }

    // 保存或更新价格
    private void saveOrUpdatePrice(String instanceId, String priceTypeCode, BigDecimal priceValue,
                                   String chargeName, String typeCode, String orgId, String ybType) {
        if (priceValue == null) {
            return;
        }
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, "adm_device_definition") //收费项目定义实体
                .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId) //业务实例主键
                .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, priceTypeCode) // 价格类型编码
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
    public boolean modifyEquipmentStatus(EquipmentStatusDTO dto) {
        String currentTenantId = userHolder.getCurrentTenantId();
//        String currentTenantId = "1";
        Boolean targetStatus = dto.getActiveFlag();
        // 批量更新限定当前租户的记录，幂等（状态相同不重复更新）
        return lambdaUpdate()
                .in(AdmDeviceDefinitionDO::getId, dto.getIds())
                .eq(AdmDeviceDefinitionDO::getTenantId, currentTenantId)
                .ne(AdmDeviceDefinitionDO::getActiveFlag, targetStatus) // 跳过已经目标状态的记录，避免无效更新
                .set(AdmDeviceDefinitionDO::getActiveFlag, targetStatus)
                .update();
    }

    /**
     * 新增器材
     *
     * @param equipmentAddDTO 新增器材的DTO
     */
    @Override
    @Transactional
    public void saveEquipment(EquipmentAddDTO equipmentAddDTO) {
        //1.DTO转Entity实体类
        AdmDeviceDefinitionDO equipmentEntity = msEquipmentMapper.saveDtoToEntity(equipmentAddDTO);
        equipmentEntity.setStatusEnum("ACTIVE");
        equipmentEntity.setActiveFlag(true);
        // 执行机构/默认存放位置由登录上下文决定，不信任前端传入
        equipmentEntity.setOrgId(userHolder.getCurrentOrgId());
        equipmentEntity.setLocationId(userHolder.getCurrentLocationId());
        AdmChargeItemDefinitionDO chargeItemDefinitionEntity = msEquipmentMapper.saveDtoToChargeItemDefinitionEntity(equipmentAddDTO);
        // 收费项目所属机构同样取登录上下文
        chargeItemDefinitionEntity.setOrgId(userHolder.getCurrentOrgId());
        //2.数据持久化
        this.baseMapper.insert(equipmentEntity);
        //3.设置关联表数据
        chargeItemDefinitionEntity.setInstanceId(equipmentEntity.getId());
        chargeItemDefinitionEntity.setInstanceTable(EquipmentConstants.EQUIPMENT_TABLE_NAME);
        chargeItemDefinitionEntity.setStatusEnum("ACTIVE");
        chargeItemDefinitionEntity.setPriceTypeCode("PURCHASE");
        chargeItemDefinitionEntity.setPriceValue(equipmentAddDTO.getPurchasePrice());
        //4.保存价格
        //4.1.保存采购价格
        chargeItemMapper.insert(chargeItemDefinitionEntity);
        //4.2.保存零售价格
        chargeItemDefinitionEntity.setId(null);
        chargeItemDefinitionEntity.setPriceTypeCode("SALE");
        chargeItemDefinitionEntity.setPriceValue(equipmentAddDTO.getRetailPrice());
        chargeItemMapper.insert(chargeItemDefinitionEntity);
        //4.3.判断是否存在最高价格，而后保存
        if (equipmentAddDTO.getMaximumRetailPrice() != null) {
            chargeItemDefinitionEntity.setId(null);
            chargeItemDefinitionEntity.setPriceTypeCode("LIMIT");
            chargeItemDefinitionEntity.setPriceValue(equipmentAddDTO.getMaximumRetailPrice());
            chargeItemMapper.insert(chargeItemDefinitionEntity);
        }
    }

    /**
     * 获取器材详情
     *
     * @param equipmentId 器材Id
     * @return 器材VO
     */
    @Override
    public EquipmentVO getEquipmentById(String equipmentId) {
        List<EquipmentVO> voList = this.baseMapper.queryEquipmentsById(equipmentId);
        if (voList == null || voList.isEmpty()) {
            return null;
        }
        EquipmentVO vo = voList.get(0);
        for (EquipmentVO temp : voList) {
            String priceTypeCode = temp.getPriceTypeCode();
            if (priceTypeCode.equals("PURCHASE")) {
                vo.setPurchasePrice(temp.getPurchasePrice());
            } else if (priceTypeCode.equals("SALE")) {
                vo.setRetailPrice(temp.getRetailPrice());
            } else {
                vo.setMaximumRetailPrice(temp.getMaximumRetailPrice());
            }
        }
        //设置器材分类名
        vo.setCategoryName(EquipmentCategoryConstants.CATEGORY_MAP.get(vo.getCategoryCode()));
        //设置器材财物类型名
        vo.setItemTypeCode_dictText(EquipmentCategoryConstants.FIN_TYPE_CODE.get(vo.getItemTypeCode()));
        //设置采购单位名
        vo.setPurchaseUnitName(EquipmentCategoryConstants.UNIT_CODE.get(vo.getPurchaseUnitCode()));
        //设置单位名
        vo.setUnitCode_dictText(EquipmentCategoryConstants.UNIT_CODE.get(vo.getUnitCode()));
        //设置剂量单位名
        vo.setDoseUnitCode_dictText(EquipmentCategoryConstants.UNIT_CODE.get(vo.getDoseUnitCode()));
        //设置拆零单位名
        vo.setSplitUnitName(EquipmentCategoryConstants.UNIT_CODE.get(vo.getSplitUnitCode()));
        //设置销售单位名
        vo.setPackageUnitName(EquipmentCategoryConstants.UNIT_CODE.get(vo.getPackageUnitCode()));
        //设置库存单位名
        vo.setStockUnitName(EquipmentCategoryConstants.UNIT_CODE.get(vo.getStockUnitCode()));
        //设置供应商名称，供应商非固定单位，后续调用其他服务获取
        vo.setSupplyId_dictText("供应商名称");
        //设置医保类型名
        vo.setYbType_dictText(EquipmentCategoryConstants.YB_TYPE_CODE.get(vo.getYbType()));
        return vo;
    }

    /**
     * 获取选择器材种类列表
     *
     * @return 可选择的器材种类列表
     */
    @Override
    public List<EquipmentCategoryVO> queryEquipmentCategoryList() {
//        List<EquipmentCategoryVO> voList = conceptMapper.queryEquipmentCategoryList(EquipmentConstants.EQUIPMENT_CATEGORY_CODE);
        List<EquipmentCategoryVO> voList = EquipmentCategoryConstants.CATEGORY_MAP
                .entrySet()
                .stream()
                .map(entry -> new EquipmentCategoryVO(entry.getKey(), entry.getValue()))
                .toList();
        return voList;
    }

    /**
     * 批量删除器材信息
     *
     * @param equipmentIds 器材Id集合
     */
    @SneakyThrows
    @Override
    @Transactional
    public void removeEquipmentByIds(List<String> equipmentIds) {
        if (equipmentIds == null || equipmentIds.isEmpty()) {
            return;
        }
//        this.baseMapper.deleteByEquipmentIds(equipmentIds, userHolder.getCurrentUser().getId());
        this.baseMapper.deleteByEquipmentIds(equipmentIds, "1");
//        chargeItemMapper.deleteByEquipmentIds(equipmentIds, userHolder.getCurrentUser().getId());
        chargeItemMapper.deleteByEquipmentIds(equipmentIds, "1");
    }
}
