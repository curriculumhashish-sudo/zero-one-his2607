package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;

/**
 * <p>
 * 描述：药品管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IMedicineService extends IService<MedMedicationDefinitionDO> {
    MedicineDetailDTO queryMedicineDetailById(String id);

    int updateMedicine(MedicineDetailDTO dto);

    int deleteMedicine(MedicineDeleteDTO dto);

    int updateChangeMedicineStatus(int status, MedicineChangeStatusDTO dto);
}
