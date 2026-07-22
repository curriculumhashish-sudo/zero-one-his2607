package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;

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

    /**
     * 获取可选择诊断列表（条件+分页）。
     *
     * @param query 查询条件
     * @return 可选择诊断分页数据
     */
    JsonVO<PageDTO<DiagnosisOptionVO>> queryDiagnosisSelectList(DiagnosisSelectPageQuery query);

    /**
     * 获取诊断管理列表（条件+分页）。
     *
     * @param query 查询条件
     * @return 诊断管理分页数据
     */
    JsonVO<PageDTO<DiagnosisListVO>> queryDiagnosisList(DiagnosisPageQuery query);

    /**
     * 获取诊断详情。
     *
     * @param id 诊断ID
     * @return 诊断详情
     */
     JsonVO<DiagnosisDetailVO> queryDiagnosisDetail(String id);

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
}
