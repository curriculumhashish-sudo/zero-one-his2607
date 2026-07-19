package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineChangeStatusVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDeleteVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：药品管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface MedicineApis {
    /**
    * <p>
    * 描述：修改药品接口
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品更新dto
    * @return 更新结果
    */
    JsonVO<String> updateMedicine(MedicineUpdateDTO dto);

    /**
    * <p>
    * 描述：删除药品接口（批量）
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品删除dto
    * @return 药品删除vo 包含删除成功药品ids
    */
    JsonVO<MedicineDeleteVO> deleteMedicine(MedicineDeleteDTO dto);

    /**
    * <p>
    * 描述：启用药品接口（批量）
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品状态转换DTO
    * @return 药品状态转换VO 包含转换状态成功的药品ids
    */
    JsonVO<MedicineChangeStatusVO> updateEnableMedicineStatus(MedicineChangeStatusDTO dto);

    /**
    * <p>
    * 描述：停用药品接口（批量）
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品状态转换DTO
    * @return 药品状态转换VO 包含转换状态成功的药品ids
    */
    JsonVO<MedicineChangeStatusVO> updateDisableMedicineStatus(MedicineChangeStatusDTO dto);
}
