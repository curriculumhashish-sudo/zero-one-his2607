package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.OrgNameDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProAddDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProListDTO;
import com.zeroone.star.project.dto.j5.ConfigurationManagement.RunProUpdateDTO;
import com.zeroone.star.project.query.j5.ConfigurationManagement.OrgNameQuery;
import com.zeroone.star.project.query.j5.ConfigurationManagement.RunProListQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 配置管理-执行地点服务接口
 */
public interface PlaceService {

    /**
     * 根据科室ID获取名称
     */
    JsonVO<OrgNameDTO> getOrgName(OrgNameQuery query);

    /**
     * 条件+分页查询执行项目
     */
    JsonVO<PageDTO<RunProListDTO>> queryRunProPage(RunProListQuery query);

    /**
     * 新增执行项目
     */
    JsonVO<String> addRunPro(RunProAddDTO dto);

    /**
     * 修改执行项目
     */
    JsonVO<String> updateRunPro(RunProUpdateDTO dto);

    /**
     * 删除执行项目
     */
    JsonVO<Void> deleteRunPro(String id);
}
