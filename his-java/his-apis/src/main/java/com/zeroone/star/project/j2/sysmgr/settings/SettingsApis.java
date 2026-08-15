package com.zeroone.star.project.j2.sysmgr.settings;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDeleteDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingsDTO;
import com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：配置项管理接口
 * </p>
 * @author L
 * @version 1.0.0
 */
public interface SettingsApis {
    /**
     * 获取配置项列表
     * @param query 查询参数
     * @return 配置项列表
     */
    JsonVO<PageDTO<SettingsDTO>> queryAll(SettingsQuery query);

    /**
     * 新增配置项
     * @param data 配置项新增对象
     * @return 新增结果
     */
    JsonVO<String> addSettings(SettingsAddDTO data);

    /**
     * 更新配置项
     *
     * @param settingUpdateDTO 更新配置项数据
     * @return 更新结果
     */
    JsonVO<String> updateSetting(SettingUpdateDTO settingUpdateDTO) throws Exception;

    /**
     * 删除配置项
     *
     * @param settingDeleteDTO 删除配置项数据
     * @return 删除结果
     */
    JsonVO<String> deleteSetting(SettingDeleteDTO settingDeleteDTO) throws Exception;
    }
