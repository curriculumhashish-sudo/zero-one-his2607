package com.zeroone.star.sysmgr.service;

import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDeleteDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingUpdateDTO;

public interface SettingsService {
    Boolean updateSetting(SettingUpdateDTO settingUpdateDTO) throws Exception;

    Boolean deleteSetting(SettingDeleteDTO settingDeleteDTO) throws Exception;
}
