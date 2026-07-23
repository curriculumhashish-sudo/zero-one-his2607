package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;

import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.basedata.mapper.MedicineMapper;
import com.zeroone.star.basedata.service.IMedicineService;
import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 描述：药品管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class MedicineServiceImpl extends ServiceImpl<MedicineMapper, MedMedicationDefinitionDO> implements IMedicineService {
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
    public int updateChangeMedicineStatus(int status, MedicineChangeStatusDTO dto) {
        return 0;
    }
}
