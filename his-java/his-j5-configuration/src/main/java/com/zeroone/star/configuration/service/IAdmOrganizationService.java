package com.zeroone.star.configuration.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.configuration.entity.AdmOrganization;
import com.zeroone.star.project.dto.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

public interface IAdmOrganizationService extends IService<AdmOrganization> {

    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery orgNameQuery);

    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery runProListQuery);
}
