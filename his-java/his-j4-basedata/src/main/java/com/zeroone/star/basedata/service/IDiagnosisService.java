package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisSaveDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisUpdateDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisIdsDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;

import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;


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
    String saveDefinition(DiagnosisSaveDTO dto);

    /**
     * 编辑诊断定义
     * @param dto 诊断定义数据
     */
    void modifyDefinition(DiagnosisUpdateDTO dto);


    /**
     * 批量修改诊断状态
     * @param dto 诊断状态数据
     */
    void modifyBatchStatus(DiagnosisStatusDTO dto);

    /**
     * 批量删除诊断
     * @param dto 诊断ID列表
     */
    void removeBatch(DiagnosisIdsDTO dto);

    /**
     * 获取选择列表（条件+分页）
     * @param query 查询条件
     * @return 诊断定义列表
     */
    Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query);

    /**
     * 获取选择诊断列表（条件+分页）。
     * @param query 查询条件
     * @return 可选择诊断分页数据
     */
    PageDTO<DiagnosisOptionVO> queryDiagnosisSelectList(DiagnosisSelectPageQuery query);

    /**
     * 获取诊断管理列表（条件+分页）。
     * @param query 查询条件
     * @return 诊断管理分页数据
     */
    PageDTO<DiagnosisListVO> queryDiagnosisList(DiagnosisPageQuery query);

    /**
     * 获取诊断详情。
     * @param id 诊断ID
     * @return 诊断详情
     */
    DiagnosisDetailVO queryDiagnosisDetail(String id);
}
