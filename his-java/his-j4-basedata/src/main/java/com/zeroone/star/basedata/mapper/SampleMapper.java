package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.sample.AdmSpecimenDefinitionDO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：样本管理数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface SampleMapper extends BaseMapper<AdmSpecimenDefinitionDO> {

    /**
     * 分页+条件查询样本定义列表（限定当前租户）
     *
     * @param page     分页对象
     * @param query    查询条件（关键词/标本种类/状态）
     * @param tenantId 当前租户ID
     * @return 样本定义分页数据
     */
    Page<SpecimenDefinitionDetailVO> selectSamplePage(
            Page<SpecimenDefinitionDetailVO> page,
            @Param("query") SpecimenDefinitionQuery query,
            @Param("tenantId") String tenantId
    );

    /**
     * 获取样本名称下拉选项（限定当前租户，仅返回未删除记录）
     *
     * @param tenantId 当前租户ID
     * @return 样本名称下拉选项列表
     */
    List<SpecimenOptionVO> selectSampleOptions(@Param("tenantId") String tenantId);
}
