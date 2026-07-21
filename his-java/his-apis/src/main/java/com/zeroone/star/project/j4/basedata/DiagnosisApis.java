package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;
import io.swagger.v3.oas.annotations.Operation;

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

}
