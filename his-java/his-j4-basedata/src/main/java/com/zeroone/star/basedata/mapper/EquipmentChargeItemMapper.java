package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.equipment.AdmChargeItemDefinitionDO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 器材收费项目定义数据访问接口
 *
 * @author j4组
 */
public interface EquipmentChargeItemMapper extends BaseMapper<AdmChargeItemDefinitionDO> {

    /**
     * 批量删除器材关联价格数据
     * @param equipmentIds 器材Id集合
     */
    void deleteByEquipmentIds(@Param("equipmentIds") List<String> equipmentIds,
                              @Param("userId") String userId);
}
