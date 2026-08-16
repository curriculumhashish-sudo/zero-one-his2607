package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;

import java.util.List;

/**
 * <p>
 * 描述：样本管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface SampleApis {

    /**
     * 启用/停用样本定义
     * 根据主键与乐观锁版本号，更新样本定义的发布状态
     * @param dto 启停用请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> updateSampleStatus(SpecimenDefinitionStatusDTO dto);

    /**
     * 删除样本定义
     * 根据主键与乐观锁版本号，软删除样本定义
     * @param dto 删除请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> deleteSample(SpecimenDefinitionDeleteDTO dto);

    /**
     * 修改样本定义
     * 根据主键与乐观锁版本号，修改样本名称、标本种类
     * @param dto 修改请求数据
     * @return 操作结果
     */
    JsonVO<SpecimenDefinitionOperateVO> updateSample(SpecimenDefinitionUpdateDTO dto);

    /**
     * 获取样本名称（下拉选项）
     * 获取当前租户下所有样本定义的名称列表，供其他组模块调用，前端渲染为下拉选项，选择后存储样本ID
     * @return 样本名称下拉选项列表
     */
    JsonVO<List<SpecimenOptionVO>> listSampleNames();

    /**
     * 获取样本列表（条件+分页）
     * 支持按名称/拼音/五笔模糊查询，按标本种类/状态精确筛选
     * @param query 查询参数
     * @return 分页样本定义列表
     */
    JsonVO<PageDTO<SpecimenDefinitionDetailVO>> querySampleList(SpecimenDefinitionQuery query);

    /**
     * 新增样本定义
     * 根据传入数据新增一条样本定义记录
     * @param dto 新增请求数据
     * @return 新增结果
     */
    JsonVO<String> addSample(SpecimenDefinitionAddDTO dto);
}
