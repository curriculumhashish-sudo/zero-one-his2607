package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;

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
    JsonVO<PageDTO<TreatmentListVO>> queryTreatmentList(TreatmentQuery query);

    /**
     * 查询可选诊疗列表
     * @param query 查询条件
     * @return 分页数据，每条含id/busNo/name/categoryCode
     */
    JsonVO<PageDTO<TreatmentSelectVO>> listTreatmentOptions(TreatmentSelectQuery query);

    /**
     * 获取诊疗详情
     * @param id 诊疗数据主键ID
     * @return 诊疗详情数据
     */
    JsonVO<TreatmentDetailVO> getTreatment(String id);

    /**
     * 获取诊疗分类选项
     * @return 分类名称列表
     */
    JsonVO<List<String>> listCategoryOptions();

    /**
     * 添加诊疗项目
     * @param treatmentAddDTO 诊疗详情信息DTO
     * @return 无返回值
     */
    JsonVO<Void> addTreatment(TreatmentAddDTO treatmentAddDTO);

    /**
     * 编辑诊疗项目（携带编辑校验码）
     * @param dto 编辑诊疗数据
     * @return 编辑确认结果
     */
    JsonVO<TreatmentDetailVO> modifyTreatment(TreatmentEditDTO dto);

    /**
     * 修改诊疗项目
     * @param dto 修改诊疗数据
     * @return 修改确认结果
     */
    JsonVO<TreatmentDetailVO> updateTreatment(TreatmentUpdateDTO dto);

    /**
     * 切换诊疗状态（启用/禁用取反）
     * @param dto 切换状态请求
     * @return 切换后结果
     */
    JsonVO<TreatmentDetailVO> modifyStatus(TreatmentStatusDTO dto);

    /**
     * 删除诊疗项目
     * @param id 诊疗项目ID
     * @return 无返回值
     */
    JsonVO<Void> removeTreatment(String id);
}
