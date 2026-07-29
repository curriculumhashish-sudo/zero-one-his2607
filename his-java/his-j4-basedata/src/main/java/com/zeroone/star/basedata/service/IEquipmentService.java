package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import jakarta.validation.Valid;

import java.math.BigDecimal;

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
public interface IEquipmentService extends IService<AdmDeviceDefinitionDO> {

    /**
     * 获取器材列表(条件+分页)
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 器材实体分页数据
     */
    Page<AdmDeviceDefinitionDO> queryEquipmentPage(EquipmentPageQuery query);

    /**
     * 修改器材
     *
     * @param dto 修改器材的DTO
     * @return 修改后的器材实体
     */
    AdmDeviceDefinitionDO updateEquipment(@Valid EquipmentUpdateDTO dto);

    /**
     * 启/停用器材（支持批量）
     *
     * @param dto 切换状态请求（主键列表）
     * @return 成功修改的数量
     */
    int modifyEquipmentStatus(@Valid EquipmentStatusDTO dto);

    /**
     * 获取器材零售价（从价格表查询）
     *
     * @param deviceId 器材主键
     * @return 零售价，未找到返回 null
     */
    BigDecimal getRetailPrice(Long deviceId);
}
