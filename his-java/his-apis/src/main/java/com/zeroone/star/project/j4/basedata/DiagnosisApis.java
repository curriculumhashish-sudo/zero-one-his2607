package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;


import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;

import com.zeroone.star.project.vo.JsonVO;

import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionVO;



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
     * 查询诊断定义详情
     * @param id 诊断定义主键
     * @return 诊断定义详情（含引用关系列表）
     */
    default JsonVO<DiagnosisDefinitionDetailVO> queryDefinitionDetail(String id) {
        return null;
    }

    /**
     * 新增诊断
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
     * 启用/停用诊断定义（支持批量操作）
     * @param ids 诊断定义主键列表
     * @param status 状态（ACTIVE/INACTIVE）
     * @return 操作结果
     */
    default JsonVO<String> modifyBatchStatus(List<String> ids, String status) {
        return null;
    }

    /**
     * 删除诊断定义（支持批量操作）
     * @param ids 诊断定义主键列表
     * @return 删除结果
     */
    default JsonVO<String> removeBatch(List<String> ids) {
        return null;
    }


    /**
     * 获取选择诊断列表（条件+分页）
     * @param query 查询条件
     * @return 诊断列表
     */
    default JsonVO<PageDTO<DiagnosisDefinitionVO>> getSelectList(DiagnosisDefinitionQuery query) {
        return null;
    }
}
