package com.zeroone.star.project.j4.diagnosis;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.BodyStructureDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisTemplateDTO;
import com.zeroone.star.project.dto.j4.diagnosis.YbMappingDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.BodyStructureTreeNodeVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisTemplateVO;
import com.zeroone.star.project.vo.j4.diagnosis.DisableCheckResultVO;

import java.util.List;

/**
 * <p>
 * 描述：诊断管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface DiagnosisApis {

    // 诊断目录维护
    /**
     * 分页查询诊断定义列表
     * @param query 查询条件（支持按名称/拼音码/五笔码/来源/类型/状态筛选）
     * @return 分页诊断定义列表
     */
    default JsonVO<PageDTO<DiagnosisDefinitionVO>> queryDefinitions(DiagnosisDefinitionQuery query) {
        return null;
    }

    /**
     * 查询诊断定义详情（含引用关系一览）
     * @param id 诊断定义主键
     * @return 诊断定义详情（含引用关系列表）
     */
    default JsonVO<DiagnosisDefinitionDetailVO> queryDefinitionDetail(String id) {
        return null;
    }

    /**
     * 新增诊断定义
     * @param dto 诊断定义数据（编码和拼音/五笔码由后端自动生成）
     * @return 新增记录的 ID
     */
    default JsonVO<String> addDefinition(DiagnosisDefinitionDTO dto) {
        return null;
    }

    /**
     * 编辑诊断定义
     * @param dto 诊断定义数据（id 必填，仅修改可编辑字段）
     * @return 编辑结果
     */
    default JsonVO<String> modifyDefinition(DiagnosisDefinitionDTO dto) {
        return null;
    }

    /**
     * 停用诊断定义前校验引用关系
     * @param id 诊断定义主键
     * @return 校验结果（可停用 / 不可停用 + 引用明细）
     */
    default JsonVO<DisableCheckResultVO> checkBeforeDisable(String id) {
        return null;
    }

    /**
     * 停用诊断定义（将状态改为 INACTIVE）
     * @param id 诊断定义主键
     * @return 停用结果
     */
    default JsonVO<String> disableDefinition(String id) {
        return null;
    }

    /**
     * 医保对照维护
     * @param dto 医保对照数据
     * @return 维护结果
     */
    default JsonVO<String> updateYbMapping(YbMappingDTO dto) {
        return null;
    }

    /**
     * 导出诊断定义数据（Excel）
     * @param query 查询条件
     * @return 导出结果
     */
    default JsonVO<String> exportDefinitions(DiagnosisDefinitionQuery query) {
        return null;
    }

    //   身体结构目录管理  

    /**
     * 查询身体结构树（递归加载整棵树）
     * @return 身体结构树节点列表（根节点集合，通过 children 递归）
     */
    default JsonVO<List<BodyStructureTreeNodeVO>> queryBodyStructureTree() {
        return null;
    }

    /**
     * 新增身体结构节点
     * @param dto 节点数据
     * @return 新增节点的 ID
     */
    default JsonVO<String> addBodyStructureNode(BodyStructureDTO dto) {
        return null;
    }

    /**
     * 编辑身体结构节点
     * @param dto 节点数据（id 必填，修改父节点时校验不能移动到自身子节点下）
     * @return 编辑结果
     */
    default JsonVO<String> modifyBodyStructureNode(BodyStructureDTO dto) {
        return null;
    }

    /**
     * 删除身体结构节点（软删除，需先校验无子节点且未被诊断引用）
     * @param id 节点主键
     * @return 删除结果
     */
    default JsonVO<String> deleteBodyStructureNode(String id) {
        return null;
    }

    //   诊断模板管理  

    /**
     * 分页查询诊断模板列表
     * @param query 查询条件（按绑定类型和归属主体筛选）
     * @return 分页模板列表
     */
    default JsonVO<PageDTO<DiagnosisTemplateVO>> queryTemplates(DiagnosisTemplateQuery query) {
        return null;
    }

    /**
     * 新增诊断模板
     * @param dto 模板数据（诊断项不可为空）
     * @return 新增模板的 ID
     */
    default JsonVO<String> addTemplate(DiagnosisTemplateDTO dto) {
        return null;
    }

    /**
     * 编辑诊断模板
     * @param dto 模板数据（id 必填）
     * @return 编辑结果
     */
    default JsonVO<String> modifyTemplate(DiagnosisTemplateDTO dto) {
        return null;
    }

    /**
     * 删除诊断模板（软删除）
     * @param id 模板主键
     * @return 删除结果
     */
    default JsonVO<String> deleteTemplate(String id) {
        return null;
    }
}
