package com.zeroone.star.project.j5.ConfigurationManagement;

import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.annotations.Api;

@Api(tags = "配置管理")
public interface ConfigurationManagementApis {
    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery);

    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery);
}
