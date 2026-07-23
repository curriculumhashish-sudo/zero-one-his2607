package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVo;
import com.zeroone.star.project.vo.j4.equipment.EquipmentUpdateVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;

import java.util.List;

/**
 * <p>
 * 描述：器材管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
public interface EquipmentApis {

    /**
     * 分页查询器材列表
     *
     * @param query 分页查询条件
     * @return 分页器材数据
     */
    JsonVO<PageDTO<EquipmentVO>> queryPage(EquipmentPageQuery query);

    /**
     * 添加器材
     *
     * @param equipmentAddDTO 器材详情信息DTO
     * @return 无返回值
     */
    JsonVO<Void> addEquipment(EquipmentAddDTO equipmentAddDTO);


    /**
     * 修改器材
     *
     * @param dto 修改器材数据
     * @return 修改确认结果
     */
    JsonVO<EquipmentUpdateVO> updateEquipment(EquipmentUpdateDTO dto);

    /**
     * 切换器材状态（启用/禁用取反）
     *
     * @param dto 切换状态请求
     * @return 切换后结果
     */
    JsonVO<EquipmentUpdateVO> modifyStatus(EquipmentStatusDTO dto);

    /**
     * 获取器材详情
     *
     * @param equipmentId 器材id
     * @return 器材详情
     */
    JsonVO<EquipmentVO> getEquipmentByEquipmentId(Long equipmentId);

    /**
     * 获取选择器材种类列表
     *
     * @return 可选择的器材种类列表
     */
    JsonVO<List<EquipmentCategoryVo>> queryEquipmentCategoryList();

    /**
     * 批量删除器材信息
     *
     * @param equipmentIds 器材id集合
     * @return 无返回值
     */
    JsonVO<Void> removeEquipmentByEquipmentIds(List<Long> equipmentIds);
}
