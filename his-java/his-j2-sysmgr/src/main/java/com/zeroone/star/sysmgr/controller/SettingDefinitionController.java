package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDefinitionDTO;
import com.zeroone.star.project.j2.sysmgr.settings.SettingDefinitionApis;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：配置管理控制器
 * </p>
 * @author L
 * @version 1.0.0
 */
@RestController
@Tag(name = "配置管理")
public class SettingDefinitionController implements SettingDefinitionApis {
    @Operation(summary = "查询所有配置")
    @GetMapping("query")
    @Override
    public JsonVO<PageDTO<SettingDefinitionDTO>> queryAll(SettingDefinitionQuery query){
        return null;
    }
}
