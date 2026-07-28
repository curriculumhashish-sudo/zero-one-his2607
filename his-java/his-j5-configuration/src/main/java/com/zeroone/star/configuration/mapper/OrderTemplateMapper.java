package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.OrderTemplate;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 组套模板头 Mapper 接口 —— 所有查询显式指定列，禁止 SELECT *
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Mapper
public interface OrderTemplateMapper extends BaseMapper<OrderTemplate> {

    /**
     * 按状态/场景/类型筛选名称列表，支持个人+科室+全院可见性过滤
     * @param statusEnum      状态（如 ACTIVE）
     * @param sceneEnum       适用场景
     * @param templateKindEnum 组套种类
     * @param practitionerId  当前医生ID（可为null）
     * @param deptId          当前科室ID（可为null）
     * @return 名称列表
     */
    List<OrderTemplate> selectNames(
            @Param("statusEnum") String statusEnum,
            @Param("sceneEnum") String sceneEnum,
            @Param("templateKindEnum") String templateKindEnum,
            @Param("practitionerId") String practitionerId,
            @Param("deptId") String deptId);

    /**
     * 分页查询组套模板列表（显式列，禁止 SELECT *）
     * @param page             分页对象
     * @param templateKindEnum 组套种类
     * @param keyword          关键词（可为null）
     * @param bindingEnum      绑定类型（可为null）
     * @param sceneEnum        适用场景（可为null）
     * @param statusEnum       状态（可为null）
     * @return 分页结果
     */
    Page<OrderTemplate> selectPageExplicit(
            Page<OrderTemplate> page,
            @Param("templateKindEnum") String templateKindEnum,
            @Param("keyword") String keyword,
            @Param("bindingEnum") String bindingEnum,
            @Param("sceneEnum") String sceneEnum,
            @Param("statusEnum") String statusEnum);
}