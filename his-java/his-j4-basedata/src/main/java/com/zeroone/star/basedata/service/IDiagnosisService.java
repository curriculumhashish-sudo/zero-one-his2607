package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.ClnConditionDefinitionDO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;

import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;


import java.util.List;

/**
 * <p>
 * 描述：诊断服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IDiagnosisService {

    /**
     * 分页查询诊断定义
     * @param query 查询条件
     * @return 分页结果
     */
    Page<ClnConditionDefinitionDO> pageQuery(DiagnosisDefinitionQuery query);

    /**
     * 根据 ID 查询诊断定义
     * @param id 主键
     * @return 诊断定义实体
     */
    ClnConditionDefinitionDO getById(String id);

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
     * 批量修改诊断状态
     * @param ids 诊断ID列表
     * @param status 状态（ACTIVE/INACTIVE）
     */
    void modifyBatchStatus(List<String> ids, String status);

    /**
     * 批量删除诊断
     * @param ids 诊断ID列表
     */
    void removeBatch(List<String> ids);

    /**
     * 获取选择列表（条件+分页）
     * @param query 查询条件
     * @return 诊断定义列表
     */
    Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query);
}