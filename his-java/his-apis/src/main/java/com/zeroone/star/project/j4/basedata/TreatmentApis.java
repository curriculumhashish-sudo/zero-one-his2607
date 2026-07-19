package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.*;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

/**
 * <p>
 * 描述：诊疗管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface TreatmentApis {

    /**
     * 分页查询诊疗项目列表
     * @param query 查询条件
     * @return 分页结果数据
     */
    JsonVO<PageDTO<TreatmentDTO>> queryTreatmentList(TreatmentQuery query);

    /**
     * 查询可选诊疗列表
     * @param query 查询条件
     * @return 分页数据，每条含id/busNo/name
     */
    JsonVO<PageDTO<TreatmentSelectDTO>> listTreatmentOptions(TreatmentSelectQuery query);

    /**
     * 获取诊疗详情
     * @param id 诊疗数据主键ID
     * @return 诊疗详情数据
     */
    JsonVO<TreatmentDetailDTO> getTreatment(String id);

    /**
     * 获取诊疗分类选项
     * @return 分类选项列表
     */
    JsonVO<List<TreatmentOptionDTO>> listCategoryOptions();
}
