package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Medicine.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.Medicine.MedMedicationDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.basedata.service.IMedicineService;
import com.zeroone.star.project.dto.j4.medicine.MedicineAddDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import com.zeroone.star.project.vo.j4.medicine.MedicineAddVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;

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

    private static final String INSTANCE_TABLE =
            "med_medication_definition";

    private static final String STATUS_ACTIVE = "ACTIVE";

    private static final String PRICE_TYPE_PURCHASE = "PURCHASE";
    private static final String PRICE_TYPE_SALE = "SALE";
    private static final String PRICE_TYPE_MAX_RETAIL = "MAX_RETAIL";

    /**
     * TODO 登录选择科室功能完成后，
     * 根据当前用户ID从Redis读取本次会话选择的科室orgId。
     */
    private static final String TEMP_ORG_ID =
            "322663105141149696";

    @Resource
    private MsMedicineMapper msMedicineMapper;

    @Resource
    private AdmChargeItemDefinitionMapper chargeItemMapper;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public MedicineAddVO addMedicine(MedicineAddDTO dto) {
        validatePrice(dto);
        validateInventoryRange(dto);

        LocalDateTime now = LocalDateTime.now();
        String medicineId = IdWorker.getIdStr();

        MedMedicationDefinitionDO medicine =
                msMedicineMapper.addDtoToEntity(dto);

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

        return msMedicineMapper.entityToAddVO(medicine);
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

        if (!StringUtils.hasText(medicine.getStatusEnum())) {
            medicine.setStatusEnum(STATUS_ACTIVE);
        }

        if (medicine.getActiveFlag() == null) {
            medicine.setActiveFlag(true);
        }
    }

    /**
     * 保存一种价格定义。
     */
    private void savePrice(MedMedicationDefinitionDO medicine, MedicineAddDTO dto, String priceType, BigDecimal priceValue, LocalDateTime now) {
        AdmChargeItemDefinitionDO price = new AdmChargeItemDefinitionDO();

        price.setId(IdWorker.getIdStr());
        price.setCreatedAt(now);
        price.setUpdatedAt(now);
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
    public MedicineDetailDTO queryMedicineDetailById(String id) {
        return null;
    }

    @Override
    public int updateMedicine(MedicineDetailDTO dto) {
        return 0;
    }

    @Override
    public int deleteMedicine(MedicineDeleteDTO dto) {
        return 0;
    }

    @Override
    public int updateChangeMedicineStatus(
            int status,
            MedicineChangeStatusDTO dto) {
        return 0;
    }
}