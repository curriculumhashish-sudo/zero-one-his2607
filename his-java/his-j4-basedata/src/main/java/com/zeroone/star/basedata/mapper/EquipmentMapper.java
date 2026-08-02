package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：器材管理数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface EquipmentMapper extends BaseMapper<AdmDeviceDefinitionDO> {

    /**
     * 获取器材详情
     * @param equipmentId 器材Id
     * @return 器材VO
     */
    List<EquipmentVO> queryEquipmentsById(String equipmentId);

    /**
     * 批量删除器材数据
     * @param equipmentIds 器材Id集合
     * @param userId 当前操作用户Id
     */
    void deleteByEquipmentIds(@Param("equipmentIds") List<String> equipmentIds,
                              @Param("userId") String userId);
}
