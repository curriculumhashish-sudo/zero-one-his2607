package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：收费项目定义数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface AdmChargeItemDefinitionMapper extends BaseMapper<AdmChargeItemDefinitionDO> {

    /**
     * 批量删除器材关联价格数据
     *
     * @param equipmentIds 器材Id集合
     */
    void deleteByEquipmentIds(@Param("equipmentIds") List<String> equipmentIds,
                              @Param("userId") String userId);
}
