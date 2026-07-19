package com.zeroone.star.project.j4.equipment;

import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVo;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;

import java.util.List;

/**
 * <p>
 * 描述：器材管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface EquipmentApis {

    /**
     * 获取器材详情
     * @param equipmentId 器材id
     * @return 器材详情
     */
    JsonVO<EquipmentVO> getEquipmentByEquipmentId(Long equipmentId);

    /**
     * 获取选择器材种类列表
     * @return 可选择的器材种类列表
     */
    JsonVO<List<EquipmentCategoryVo>> getEquipmentCategoryList();

    /**
     * 批量删除器材信息
     * @param equipmentIds 器材id集合
     * @return 无返回值
     */
    JsonVO<Void> removeEquipmentByEquipmentIds(List<Long> equipmentIds);

    /**
     * 添加器材
     * @param equipmentAddDTO 器材详情信息DTO
     * @return 无返回值
     */
    JsonVO<Void> addEquipment(EquipmentAddDTO equipmentAddDTO);
}
