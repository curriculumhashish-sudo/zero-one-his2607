package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.Medicine;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：药品管理数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface MedicineMapper extends BaseMapper<MedMedicationDefinitionDO> {
}
