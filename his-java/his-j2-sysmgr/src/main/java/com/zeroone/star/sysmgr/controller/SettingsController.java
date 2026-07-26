package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j2.sysmgr.settingdto.*;
import com.zeroone.star.project.j2.sysmgr.settings.SettingDefinitionApis;
import com.zeroone.star.project.j2.sysmgr.settings.SettingsApis;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.SettingsService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：配置项管理控制器
 * </p>
 * @author L
 * @version 1.0.0
 */

@RestController
@Tag(name = "设置管理")
@RequiredArgsConstructor
public class SettingsController implements SettingsApis, SettingDefinitionApis {

    private final SettingsService settingsService;

    @Operation(summary = "获取配置项列表")
    @GetMapping("query-all")
    @Override
    public JsonVO<PageDTO<SettingsDTO>> queryAll(SettingsQuery query) {
        return null;
    }

    @Operation(summary = "获取配置列表",description = "支持通过关键字进行模糊筛选")
    @GetMapping("query")
    @Override
    public JsonVO<PageDTO<SettingDefinitionDTO>> queryAll(SettingDefinitionQuery query){
        return null;
    }
    @Operation(summary = "新增配置项")
    @PostMapping("add-settings")
    @Override
    public JsonVO<String> addSettings(@RequestBody SettingsAddDTO data) {
        return null;
    }

    @Operation(summary = "更新配置项")
    @PutMapping("setting-update")
    @Override
    public JsonVO<String> updateSetting(@RequestBody SettingUpdateDTO settingUpdateDTO) throws Exception {
        Boolean result = settingsService.updateSetting(settingUpdateDTO);

        if(!result){
            return JsonVO.success("更新失败");
        }
        return JsonVO.success("更新成功");
    }

    @Operation(summary = "删除配置项")
    @DeleteMapping("setting-delete")
    @Override
    public JsonVO<String> deleteSetting(@RequestBody SettingDeleteDTO settingDeleteDTO) throws Exception {
        Boolean result = settingsService.deleteSetting(settingDeleteDTO);

        if(!result){
            return JsonVO.success("更新失败");
        }
        return JsonVO.success("更新成功");
    }
}
