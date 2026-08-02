package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDefinitionDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDeleteDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsDTO;
import com.zeroone.star.project.j2.sysmgr.settings.SettingDefinitionApis;
import com.zeroone.star.project.j2.sysmgr.settings.SettingsApis;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.SettingsService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：系统配置项控制器
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
@Slf4j
@RestController
@RequiredArgsConstructor
@RequestMapping("settings")
@Tag(name = "系统配置管理")
public class SettingsController implements SettingDefinitionApis, SettingsApis {

    @Resource
    private SettingsService settingsService;

    private final SettingsService settingsService2;


    // ==================== 获取配置定义列表 (SettingDefinitionApis) ====================

    @Override
    @GetMapping("definitions")
    @Operation(summary = "获取配置列表")
    public JsonVO<PageDTO<SettingDefinitionDTO>> queryAll(SettingDefinitionQuery query) {
        PageDTO<SettingDefinitionDTO> page = settingsService.listDefinitions(query);
        return JsonVO.success(page);
    }

    // ==================== 获取配置项列表 (SettingsApis) ====================

    @Override
    @GetMapping
    @Operation(summary = "获取配置项列表")
    public JsonVO<PageDTO<SettingsDTO>> queryAll(SettingsQuery query) {
        if (query.getPageIndex() <= 0) {
            query.setPageIndex(1);
        }
        if (query.getPageSize() <= 0) {
            query.setPageSize(10);
        }
        PageDTO<SettingsDTO> page = settingsService.listSettings(query);
        return JsonVO.success(page);
    }

    // ==================== 新增配置项 (SettingsApis) ====================

    @Override
    @PostMapping
    @Operation(summary = "新增配置项")
    public JsonVO<String> addSettings(@RequestBody @Valid SettingsAddDTO data) {
        String errMsg = settingsService.addSetting(data);
        if (errMsg != null) {
            return JsonVO.fail(errMsg);
        }
        return JsonVO.success("新增配置项成功");
    }

    // ==================== 以下仅提供接口 ====================

    @Operation(summary = "更新配置项")
    @PutMapping("setting-update")
    @Override
    public JsonVO<String> updateSetting(@RequestBody SettingUpdateDTO settingUpdateDTO) throws Exception {
        Boolean result = settingsService2.updateSetting(settingUpdateDTO);

        if(!result){
            return JsonVO.success("更新失败");
        }
        return JsonVO.success("更新成功");
    }

    @Operation(summary = "删除配置项")
    @DeleteMapping("setting-delete")
    @Override
    public JsonVO<String> deleteSetting(@RequestBody SettingDeleteDTO settingDeleteDTO) throws Exception {
        Boolean result = settingsService2.deleteSetting(settingDeleteDTO);

        if(!result){
            return JsonVO.success("更新失败");
        }
        return JsonVO.success("更新成功");
    }
}
