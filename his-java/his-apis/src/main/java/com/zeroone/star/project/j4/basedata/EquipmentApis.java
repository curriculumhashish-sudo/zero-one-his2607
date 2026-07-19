package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentEditDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVo;
import com.zeroone.star.project.vo.j4.equipment.EquipmentUpdateVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import io.swagger.v3.oas.annotations.tags.Tag;

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
@Tag(name = "器材管理", description = "器材管理相关接口")
public interface EquipmentApis {

    /**
     * 分页查询器材列表
     *
     * @param query 分页查询条件
     * @return 分页器材数据
     */
    JsonVO<PageDTO<EquipmentVO>> queryPage(EquipmentPageQuery query);

    /**
     * 新增器材
     *
     * @param dto 新增器材数据
     * @return 新增记录主键
     */
    JsonVO<String> add(EquipmentAddDTO dto);

    /**
     * 编辑器材（携带编辑校验码）
     *
     * @param dto 编辑器材数据
     * @return 编辑确认结果
     */
    JsonVO<EquipmentUpdateVO> edit(EquipmentEditDTO dto);

    /**
     * 修改器材
     *
     * @param dto 修改器材数据
     * @return 修改确认结果
     */
    JsonVO<EquipmentUpdateVO> update(EquipmentUpdateDTO dto);

    /**
     * 切换器材状态（启用/禁用取反）
     *
     * @param dto 切换状态请求
     * @return 切换后结果
     */
    JsonVO<EquipmentUpdateVO> toggleStatus(EquipmentStatusDTO dto);

    /**
     * 获取器材详情
     *
     * @param id 器材主键
     * @return 器材详情
     */
    JsonVO<EquipmentVO> getDetail(String id);

    /**
     * 获取器材分类下拉列表
     *
     * @return 器材分类列表
     */
    JsonVO<List<EquipmentCategoryVo>> getCategories();

    /**
     * 删除器材
     *
     * @param id 器材主键
     * @return 删除结果
     */
    JsonVO<String> delete(String id);
}
