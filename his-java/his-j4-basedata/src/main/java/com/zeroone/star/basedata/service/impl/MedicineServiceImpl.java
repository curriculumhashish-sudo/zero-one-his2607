package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;

import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDispenseDO;
import com.zeroone.star.basedata.entity.medicine.MedMedicationRequestDO;
import com.zeroone.star.basedata.mapper.MedMedicationDispenseMapper;
import com.zeroone.star.basedata.mapper.MedMedicationRequestMapper;
import com.zeroone.star.basedata.service.IMedicineService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j4.medicine.*;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j4.medicine.MedicinePageQuery;
import com.zeroone.star.project.query.j4.medicine.MedicineSelectPageQuery;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineSelectVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：药品管理服务实现。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class MedicineServiceImpl extends ServiceImpl<MedMedicationDefinitionMapper, MedMedicationDefinitionDO> implements IMedicineService {

    private static final String INSTANCE_TABLE = "med_medication_definition";

    private static final String STATUS_ACTIVE = "ACTIVE";

    private static final String PRICE_TYPE_PURCHASE = "PURCHASE";
    private static final String PRICE_TYPE_SALE = "SALE";
    private static final String PRICE_TYPE_MAX_RETAIL = "MAX_RETAIL";

    /**
     * TODO 登录选择科室功能完成后，
     * 根据当前用户ID从Redis读取本次会话选择的科室orgId。
     */
    private static final String TEMP_ORG_ID = "322663105141149696";

    /**
     * TODO 用户药房选择功能完成后，
     * 从当前登录状态中获取对应的药房或库房locationId。
     */
    private static final String TEMP_LOCATION_ID = "322954949549363200";

    @Resource
    private MsMedicineMapper msMedicineMapper;

    @Resource
    private AdmChargeItemDefinitionMapper chargeItemMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    public PageDTO<MedicineListVO> queryMedicinePage(MedicinePageQuery query) {
        Page<MedicineListVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<MedicineListVO> result = baseMapper.selectMedicinePage(page, query);
        return PageDTO.create(result);
    }

    @Override
    public PageDTO<MedicineSelectVO> queryMedicineSelectPage(MedicineSelectPageQuery query) {
        Page<MedicineSelectVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<MedicineSelectVO> result = baseMapper.selectMedicineSelectPage(page, query, TEMP_LOCATION_ID);
        return PageDTO.create(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public MedicineListVO addMedicine(MedicineAddDTO dto) {
        validatePrice(dto);
        validateInventoryRange(dto);

        LocalDateTime now = LocalDateTime.now();
        String medicineId = IdWorker.getIdStr();

        MedMedicationDefinitionDO medicine = msMedicineMapper.addDtoToEntity(dto);

        fillMedicineFields(medicine, medicineId, now);

        int medicineRows = baseMapper.insert(medicine);
        if (medicineRows != 1) {
            throw new IllegalStateException("新增药品主体失败");
        }

        savePrice(medicine, dto, PRICE_TYPE_PURCHASE, dto.getPurchasePrice(), now);

        savePrice(medicine, dto, PRICE_TYPE_SALE, dto.getRetailPrice(), now);

        if (dto.getMaximumRetailPrice() != null) {
            savePrice(medicine, dto, PRICE_TYPE_MAX_RETAIL, dto.getMaximumRetailPrice(), now);
        }

        MedicineListVO vo = msMedicineMapper.entityToListVO(medicine);
        vo.setRetailPrice(dto.getRetailPrice());
        return vo;
    }

    /**
     * 补充不允许由前端控制的系统字段。
     */
    private void fillMedicineFields(MedMedicationDefinitionDO medicine, String medicineId, LocalDateTime now) {
        medicine.setId(medicineId);
        medicine.setBusNo("MD" + medicineId);
        medicine.setCreatedAt(now);
        medicine.setUpdatedAt(now);
        medicine.setIsDeleted(false);
        medicine.setVersion(0);
        medicine.setOrgId(TEMP_ORG_ID);

        // 新增药品统一以启用状态保存，状态不由前端传入。
        medicine.setStatusEnum(STATUS_ACTIVE);
        medicine.setActiveFlag(true);
    }

    /**
     * 保存一种价格定义。
     */
    private void savePrice(MedMedicationDefinitionDO medicine, MedicineAddDTO dto, String priceType, BigDecimal priceValue, LocalDateTime now) {
        AdmChargeItemDefinitionDO price = new AdmChargeItemDefinitionDO();

        price.setId(IdWorker.getIdStr());
        price.setIsDeleted(false);
        price.setVersion(0);

        price.setInstanceTable(INSTANCE_TABLE);
        price.setInstanceId(medicine.getId());
        price.setChargeName(medicine.getMerchandiseName());
        price.setStatusEnum(STATUS_ACTIVE);
        price.setOrgId(medicine.getOrgId());

        price.setTypeCode(dto.getTypeCode());
        price.setYbType(dto.getYbType());
        price.setPriceTypeCode(priceType);
        price.setPriceValue(priceValue);
        price.setPriority(0);

        int priceRows = chargeItemMapper.insert(price);
        if (priceRows != 1) {
            throw new IllegalStateException(
                    "新增药品价格失败，价格类型：" + priceType
            );
        }
    }

    /**
     * 校验价格之间的业务关系。
     */
    private void validatePrice(MedicineAddDTO dto) {
        if (dto.getMaximumRetailPrice() != null && dto.getMaximumRetailPrice().compareTo(dto.getRetailPrice()) < 0) {
            throw new IllegalArgumentException("最高零售价不能小于零售价");
        }
    }

    /**
     * 库存校验
     */
    private void validateInventoryRange(MedicineAddDTO dto) {
        if (dto.getItemMinQuantity() != null
                && dto.getItemMaxQuantity() != null
                && dto.getItemMaxQuantity()
                .compareTo(dto.getItemMinQuantity()) < 0) {
            throw new IllegalArgumentException(
                    "库存警戒线最高值不能小于最低值"
            );
        }
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public MedicineDetailDTO queryMedicineDetailById(String id) {
        //获取租户id
        String tenantId = userHolder.getCurrentTenantId();

        MedMedicationDefinitionDO medicationDefinitionDO = baseMapper.selectById(id);
        //获取与id值关联的AdmChargeItemDefinitionDO列表
        QueryWrapper<AdmChargeItemDefinitionDO> queryWrapper = new QueryWrapper<>();
        queryWrapper.lambda()
                .eq(AdmChargeItemDefinitionDO::getInstanceTable,"med_medication_definition")
                .eq(AdmChargeItemDefinitionDO::getInstanceId,id)
                .eq(AdmChargeItemDefinitionDO::getTenantId,tenantId);//补充租户条件

        List<AdmChargeItemDefinitionDO> chargeItemDefinitionDOS = chargeItemMapper.selectList(queryWrapper);
        if(medicationDefinitionDO == null||chargeItemDefinitionDOS ==null||chargeItemDefinitionDOS.isEmpty()){
            return null;
        }
        //简单组装MedicineDatailDTO
        MedicineDetailDTO detailDTO = msMedicineMapper.entityToDetailDTO(medicationDefinitionDO);
        //组装DTO的charge部分
        for (AdmChargeItemDefinitionDO definitionDO :chargeItemDefinitionDOS){
            String code = definitionDO.getPriceTypeCode();
            if(detailDTO.getInsuranceCategoryCode()==null && definitionDO.getYbType() != null){
                detailDTO.setInsuranceCategoryCode(definitionDO.getYbType());
            }
            if("PURCHASE".equals(code)){
                detailDTO.setPurchasePrice(definitionDO.getPriceValue());
                continue;
            }
            if ("SALE".equals(code)){
                detailDTO.setRetailPrice(definitionDO.getPriceValue());
                continue;
            }
            if("LIMIT".equals(code)){
                detailDTO.setMaxRetailPrice(definitionDO.getPriceValue());
            }
        }
        //TODO 考虑将其存入Redis
        return detailDTO;


    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public int updateMedicine(MedicineDetailDTO dto) {
        //获取租户id
        String tenantId = userHolder.getCurrentTenantId();

        MedMedicationDefinitionDO definitionDO = msMedicineMapper.detailDtoToEntity(dto);
        UpdateWrapper<MedMedicationDefinitionDO> updateWrapper = new UpdateWrapper<>();
        updateWrapper.lambda()
                .eq(MedMedicationDefinitionDO::getTenantId,tenantId)
                .eq(MedMedicationDefinitionDO::getId,definitionDO.getId());
        int i = baseMapper.update(definitionDO, updateWrapper);
        if(i==0) log.debug("definition表未更新");

        if (dto.getPurchasePrice() != null) {
            UpdateWrapper<AdmChargeItemDefinitionDO> purchaseWrapper = new UpdateWrapper<>();
            purchaseWrapper.lambda()
                    .eq(AdmChargeItemDefinitionDO::getInstanceTable, "med_medication_defination")
                    .eq(AdmChargeItemDefinitionDO::getInstanceId, dto.getId())
                    .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, "PURCHASE")
                    .eq(AdmChargeItemDefinitionDO::getTenantId,tenantId) //补充租户id条件
                    .set(AdmChargeItemDefinitionDO::getPriceValue, dto.getPurchasePrice());
            chargeItemMapper.update(null, purchaseWrapper);
        }

        if (dto.getRetailPrice() != null) {
            UpdateWrapper<AdmChargeItemDefinitionDO> saleWrapper = new UpdateWrapper<>();
            saleWrapper.lambda()
                    .eq(AdmChargeItemDefinitionDO::getInstanceTable, "med_medication_defination")
                    .eq(AdmChargeItemDefinitionDO::getInstanceId, dto.getId())
                    .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, "SALE")
                    .eq(AdmChargeItemDefinitionDO::getTenantId,tenantId) //补充租户id条件
                    .set(AdmChargeItemDefinitionDO::getPriceValue, dto.getRetailPrice());
            chargeItemMapper.update(null, saleWrapper);
        }

        if (dto.getMaxRetailPrice() != null) {
            UpdateWrapper<AdmChargeItemDefinitionDO> limitWrapper = new UpdateWrapper<>();
            limitWrapper.lambda()
                    .eq(AdmChargeItemDefinitionDO::getInstanceTable, "med_medication_defination")
                    .eq(AdmChargeItemDefinitionDO::getInstanceId, dto.getId())
                    .eq(AdmChargeItemDefinitionDO::getPriceTypeCode, "LIMIT")
                    .eq(AdmChargeItemDefinitionDO::getTenantId,tenantId) //补充租户id条件
                    .set(AdmChargeItemDefinitionDO::getPriceValue, dto.getMaxRetailPrice());
            chargeItemMapper.update(null, limitWrapper);
        }
        dto.setVersion(dto.getVersion()+1);
        // TODO Redis清理旧缓存


        return i;
    }

    @Resource
    MedMedicationDispenseMapper dispenseMapper;

    @Resource
    MedMedicationRequestMapper requestMapper;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public MedicineDeleteResultDTO deleteMedicine(MedicineDeleteDTO dto) {
        //获取租户id
        String tenantId = userHolder.getCurrentTenantId();

        List<String> ids = dto.getIds();
        //查出所有有关联的id
        LambdaQueryWrapper<MedMedicationDispenseDO> dispenseWrapper = new LambdaQueryWrapper<>();
        dispenseWrapper.in(MedMedicationDispenseDO::getMedicationId,ids)
                .eq(MedMedicationDispenseDO::getTenantId,tenantId);//补充租户条件
        Set<String> dispenseIds = dispenseMapper.selectList(dispenseWrapper).stream()
                .map(MedMedicationDispenseDO::getMedicationId)
                .collect(Collectors.toSet());

        LambdaQueryWrapper<MedMedicationRequestDO> requestWrapper = new LambdaQueryWrapper<>();
        requestWrapper.in(MedMedicationRequestDO::getMedicationId,ids)
                .eq(MedMedicationRequestDO::getTenantId,tenantId);//补充租户条件
        Set<String> requestIds = requestMapper.selectList(requestWrapper).stream()
                .map(MedMedicationRequestDO::getMedicationId)
                .collect(Collectors.toSet());
        // 合并有关联的id
        Set<String> relatedIds = new HashSet<>();
        relatedIds.addAll(dispenseIds);
        relatedIds.addAll(requestIds);

        //统计返回数据
        int successCount = 0;
        List<String> failIds = new ArrayList<>();
        List<String> deleteIds = new ArrayList<>();

        for (String id : ids) {
            if(relatedIds.contains(id)){
                failIds.add(id);
            }else {
                deleteIds.add(id);
            }

        }

        //删除满足条件的ids
        if (!deleteIds.isEmpty()){
            LambdaQueryWrapper<MedMedicationDefinitionDO> deleteWrapper = new LambdaQueryWrapper<>();
            deleteWrapper.in(MedMedicationDefinitionDO::getId,deleteIds)
                    .eq(MedMedicationDefinitionDO::getTenantId,tenantId);//补充租户条件
            int i = baseMapper.delete(deleteWrapper);
            successCount = i;
        }
        return new MedicineDeleteResultDTO(successCount, failIds.size(), failIds);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public int updateChangeMedicineStatus(
            int status,
            MedicineChangeStatusDTO dto) {
        //获取租户id
        String tenantId = userHolder.getCurrentTenantId();
        int count = 0;
        List<String> ids = dto.getIds();

        LambdaUpdateWrapper<MedMedicationDefinitionDO> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.in(MedMedicationDefinitionDO::getId,ids)
                .ne(MedMedicationDefinitionDO::getActiveFlag,status)
                .set(MedMedicationDefinitionDO::getActiveFlag,status)
                .eq(MedMedicationDefinitionDO::getTenantId,tenantId);//补充租户条件

        return baseMapper.update(null,updateWrapper);

    }
}
