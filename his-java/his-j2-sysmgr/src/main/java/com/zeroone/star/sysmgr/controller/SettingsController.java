package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsDTO;
import com.zeroone.star.project.j2.sysmgr.settings.SettingsApis;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：配置项管理控制器
 * </p>
 * @author L
 * @version 1.0.0
 */

@RestController
@Tag(name = "设置管理")
public class SettingsController implements SettingsApis {
    @Operation(summary = "获取配置项列表")
    @GetMapping("query-all")
    @Override
    public JsonVO<PageDTO<SettingsDTO>> queryAll(SettingsQuery query) {
        return null;
    }

    @Operation(summary = "新增配置项")
    @PostMapping("add-settings")
    @Override
    public JsonVO<String> addSettings(@RequestBody SettingsAddDTO data) {
        return null;
    }
}
