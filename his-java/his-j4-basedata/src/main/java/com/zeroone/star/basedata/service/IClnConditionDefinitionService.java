package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.ClnConditionDefinition;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.YbMappingDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DisableCheckResultVO;

import java.util.List;

/**
 * <p>
 * 描述：临床条件定义（诊断目录）服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IClnConditionDefinitionService {

    /**
     * 分页查询诊断定义
     * @param query 查询条件
     * @return 分页结果
     */
    Page<ClnConditionDefinition> pageQuery(DiagnosisDefinitionQuery query);

    /**
     * 根据 ID 查询诊断定义
     * @param id 主键
     * @return 诊断定义实体
     */
    ClnConditionDefinition getById(String id);

    /**
     * 查询诊断定义的引用关系列表
     * @param id 诊断定义 ID
     * @return 引用明细列表
     */
    List<DiagnosisDefinitionDetailVO.ReferenceDetail> getReferences(String id);

    /**
     * 新增诊断定义
     * @param dto 诊断定义数据
     * @return 新增记录 ID
     */
    String saveDefinition(DiagnosisDefinitionDTO dto);

    /**
     * 编辑诊断定义
     * @param dto 诊断定义数据
     */
    void modifyDefinition(DiagnosisDefinitionDTO dto);

    /**
     * 停用前查询引用关系校验
     * @param id 诊断定义 ID
     * @return 校验结果
     */
    DisableCheckResultVO queryDisableCheck(String id);

    /**
     * 修改诊断定义为停用状态
     * @param id 诊断定义 ID
     */
    void modifyDisableDefinition(String id);

    /**
     * 医保对照维护
     * @param dto 医保对照数据
     */
    void updateYbMapping(YbMappingDTO dto);
}
