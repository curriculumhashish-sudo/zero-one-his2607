package com.zeroone.star.project.j2.sysmgr.settings;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDefinitionDTO;

import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;

/**
 * <p>
 * 描述：配置管理接口
 * </p>
 * @author L
 * @version 1.0.0
 */
public interface SettingDefinitionApis {
    /**
     * 查询所有配置
     * @param query 查询参数
     * @return 配置分页结果
     */
    JsonVO<PageDTO<SettingDefinitionDTO>> queryAll(SettingDefinitionQuery query);

}
