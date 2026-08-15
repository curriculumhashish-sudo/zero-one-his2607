package com.zeroone.star.sysmgr.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.*;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingDefinitionQuery;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;

/**
 * <p>
 * 描述：系统配置项服务接口
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
public interface SettingsService {

    // ============ 获取配置定义列表 ============
    PageDTO<SettingDefinitionDTO> listDefinitions(SettingDefinitionQuery query);

    // ============ 获取配置项列表 ============
    PageDTO<SettingsDTO> listSettings(SettingsQuery query);

    // ============ 新增配置项 ============
    String addSetting(SettingsAddDTO dto);

    Boolean updateSetting(SettingUpdateDTO settingUpdateDTO) throws Exception;

    Boolean deleteSetting(SettingDeleteDTO settingDeleteDTO) throws Exception;
}
