package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.basedata.mapper.EquipmentMapper;
import com.zeroone.star.basedata.service.IEquipmentService;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

/**
 * <p>
 * 描述：器材管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Service
public class EquipmentServiceImpl extends ServiceImpl<EquipmentMapper, AdmDeviceDefinitionDO> implements IEquipmentService {

    @Resource
    private MsEquipmentMapper msEquipmentMapper;

    /**
     * 获取器材列表（条件+分页）
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 器材信息分页列表
     */
    @Override
    public Page<AdmDeviceDefinitionDO> queryEquipmentPage(EquipmentPageQuery query) {
        // 1.构建查询条件
        LambdaQueryWrapper<AdmDeviceDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getKeyword())) {
            // 关键词查询
            wrapper.and(w -> w.like(AdmDeviceDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(AdmDeviceDefinitionDO::getBusNo, query.getKeyword()));
        }

        // 2.分类筛选
        if (StringUtils.hasText(query.getCategoryCode())) {
            wrapper.eq(AdmDeviceDefinitionDO::getCategoryCode, query.getCategoryCode());
        }

        // 3.排序
        wrapper.orderByAsc(AdmDeviceDefinitionDO::getBusNo);

        // 4.构建分页对象
        Page<AdmDeviceDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // 5.执行分页查询
        return baseMapper.selectPage(page, wrapper);
    }

    /**
     * 修改器材
     *
     * @param dto 修改器材的DTO
     * @return 修改后的器材实体
     * @throws IllegalArgumentException 器材不存在或参数非法
     */
    @Override
    public AdmDeviceDefinitionDO updateEquipment(EquipmentUpdateDTO dto) {
        String id = dto.getId();
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("修改器材时主键不可为空");
        }

        AdmDeviceDefinitionDO existing = baseMapper.selectById(Long.valueOf(id));
        if (existing == null) {
            throw new IllegalArgumentException("器材不存在，主键：" + id);
        }

        AdmDeviceDefinitionDO dtoEntity = msEquipmentMapper.updateDtoToEntity(dto);
        cn.hutool.core.bean.BeanUtil.copyProperties(dtoEntity, existing,
                cn.hutool.core.bean.copier.CopyOptions.create().ignoreNullValue());
        baseMapper.updateById(existing);

        return existing;
    }

    /**
     * 启/停用器材（支持批量）
     *
     * @param dto 切换状态请求（仅主键）
     * @return 切换后的器材实体
     * @throws IllegalArgumentException 器材不存在或参数非法
     */
    @Override
    public AdmDeviceDefinitionDO modifyEquipmentStatus(EquipmentStatusDTO dto) {
        String id = dto.getId();
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("切换状态时主键不可为空");
        }

        AdmDeviceDefinitionDO existing = baseMapper.selectById(Long.valueOf(id));
        if (existing == null) {
            throw new IllegalArgumentException("器材不存在，主键：" + id);
        }

        boolean newStatus = existing.getActiveFlag() == null || !existing.getActiveFlag();
        existing.setActiveFlag(newStatus);
        baseMapper.updateById(existing);

        return existing;
    }
}
