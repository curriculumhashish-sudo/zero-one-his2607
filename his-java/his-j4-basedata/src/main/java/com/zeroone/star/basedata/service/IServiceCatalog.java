package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogAddDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogEditDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogUpdateDTO;
import com.zeroone.star.project.query.j4.treatment.ServiceCatalogQuery;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogDetailVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogListVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogSelectVO;

import java.util.List;

/**
 * <p>
 * 描述：诊疗管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IServiceCatalog extends IService<WkfActivityDefinitionDO> {

    PageDTO<ServiceCatalogSelectVO> listServiceCatalogOptions(ServiceCatalogQuery query);

    PageDTO<ServiceCatalogListVO> queryServiceCatalogList(ServiceCatalogQuery query);

    ServiceCatalogDetailVO getServiceCatalogById(String id);

    List<String> listCategoryOptions();

    void addServiceCatalog(ServiceCatalogAddDTO serviceCatalogAddDTO);

    ServiceCatalogDetailVO modifyServiceCatalog(ServiceCatalogEditDTO dto);

    ServiceCatalogDetailVO updateServiceCatalog(ServiceCatalogUpdateDTO dto);

    ServiceCatalogDetailVO modifyStatus(ServiceCatalogStatusDTO dto);

    void removeServiceCatalog(String id);
}
