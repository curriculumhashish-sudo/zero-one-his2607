package com.zeroone.star.project.j5.ConfigurationManagement;

import com.zeroone.star.project.dto.j5.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j5.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.j5.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface ConfigurationManagementApis {
    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery);

    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery);
}
