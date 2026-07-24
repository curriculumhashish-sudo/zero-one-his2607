package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinition;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import com.zeroone.star.project.vo.j4.equipment.EquipmentUpdateVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import jakarta.validation.Valid;

/**
 * <p>
 * 描述：器材管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
public interface IEquipmentService extends IService<AdmDeviceDefinition> {

    /**
     * 获取器材列表(条件+分页)
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 器材信息分页列表
     */
    PageDTO<EquipmentVO> queryEquipmentPage(EquipmentPageQuery query);

    /**
     * 修改器材
     *
     * @param dto 修改器材的DTO
     * @return 修改后的器材信息
     */
    EquipmentUpdateVO updateEquipment(@Valid EquipmentUpdateDTO dto);

    /**
     * 启/停用器材（支持批量）
     *
     * @param dto 切换状态请求（仅主键）
     * @return 切换后的器材状态信息
     */
    EquipmentVO modifyEquipmentStatus(@Valid EquipmentStatusDTO dto);
}
