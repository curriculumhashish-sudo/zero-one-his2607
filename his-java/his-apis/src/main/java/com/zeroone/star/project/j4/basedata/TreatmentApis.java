package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogAddDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogEditDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogUpdateDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogStatusDTO;
import com.zeroone.star.project.query.j4.treatment.ServiceCatalogQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogDetailVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogListVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogSelectVO;

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
    JsonVO<PageDTO<ServiceCatalogListVO>> queryTreatmentList(ServiceCatalogQuery query);

    /**
     * 查询可选诊疗列表
     * @param query 查询条件
     * @return 分页数据，每条含id/busNo/name/categoryCode
     */
    JsonVO<PageDTO<ServiceCatalogSelectVO>> listTreatmentOptions(ServiceCatalogQuery query);

    /**
     * 获取诊疗详情
     * @param id 诊疗数据主键ID
     * @return 诊疗详情数据
     */
    JsonVO<ServiceCatalogDetailVO> getTreatment(String id);

    /**
     * 获取诊疗分类选项
     * @return 分类名称列表
     */
    JsonVO<List<String>> listCategoryOptions();

    /**
     * 添加诊疗项目
     * @param serviceCatalogAddDTO 诊疗详情信息DTO
     * @return 无返回值
     */
    JsonVO<Void> addTreatment(ServiceCatalogAddDTO serviceCatalogAddDTO);

    /**
     * 编辑诊疗项目（携带编辑校验码）
     * @param dto 编辑诊疗数据
     * @return 编辑确认结果
     */
    JsonVO<ServiceCatalogDetailVO> modifyTreatment(ServiceCatalogEditDTO dto);

    /**
     * 修改诊疗项目
     * @param dto 修改诊疗数据
     * @return 修改确认结果
     */
    JsonVO<ServiceCatalogDetailVO> updateTreatment(ServiceCatalogUpdateDTO dto);

    /**
     * 切换诊疗状态（启用/禁用取反）
     * @param dto 切换状态请求
     * @return 切换后结果
     */
    JsonVO<ServiceCatalogDetailVO> modifyStatus(ServiceCatalogStatusDTO dto);

    /**
     * 删除诊疗项目
     * @param id 诊疗项目ID
     * @return 无返回值
     */
    JsonVO<Void> removeTreatment(String id);
}
