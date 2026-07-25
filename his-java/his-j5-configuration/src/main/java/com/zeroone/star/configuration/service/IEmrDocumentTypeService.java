package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.entity.EmrDocumentTypeDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j5.configuration_management.medical_record_template.EmrDocumentTypeQuery;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeDetailVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeNameVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeVO;

import java.util.List;

/**
 * <p>
 * 描述：获取文书类型service
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */
public interface IEmrDocumentTypeService extends IService<EmrDocumentTypeDO> {

    /**
     * 获取文书类型下拉字典（仅启用）
     * @return 文书类型下拉列表；无数据时返回空列表
     */
    List<EmrDocumentTypeNameVO> getEmrDocumentTypeName();

    /**
     * 分页查询文书类型列表（含启用与禁用，用于后台管理）
     * @param query 分页与条件查询参数
     * @return 分页结果 PageDTO
     */
    PageDTO<EmrDocumentTypeVO> queryEmrDocumentType(EmrDocumentTypeQuery query);

    /**
     * 根据主键查询单个文书类型详情
     * @param id 文书类型主键 ID
     * @return 文书类型详情 EmrDocumentTypeDetailVO
     */
    EmrDocumentTypeDetailVO getEmrDocumentTypeDetail(String id);
}
