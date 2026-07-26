package com.zeroone.star.sysmgr.service.impl;

import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.sysmgr.entity.WhaleSettings;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingDeleteDTO;
import com.zeroone.star.project.dto.j2.sysmgr.settingdto.SettingUpdateDTO;
import com.zeroone.star.sysmgr.mapper.SettingsMapper;
import com.zeroone.star.sysmgr.service.SettingsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;

@Service
public class SettingsServiceImpl implements SettingsService {

    @Autowired
    private SettingsMapper settingsMapper;
    @Autowired
    private UserHolder userHolder;

    @Override  //更新配置项
    @Transactional
    public Boolean updateSetting(SettingUpdateDTO settingUpdateDTO) throws Exception {
        UserDTO currentUser = userHolder.getCurrentUser();

        LambdaUpdateWrapper<WhaleSettings> wrapper = new LambdaUpdateWrapper<>();

        wrapper.eq(WhaleSettings::getId,settingUpdateDTO.getId())
                        .eq(WhaleSettings::getIsDeleted,0)
                                .set(WhaleSettings::getUpdatedAt,LocalDateTime.now())
                                        .set(WhaleSettings::getUpdatedBy,currentUser.getId());

        return settingsMapper.update(wrapper) > 0;
    }

    @Override  //删除配置项
    @Transactional
    public Boolean deleteSetting(SettingDeleteDTO settingDeleteDTO) throws Exception {
        UserDTO currentUser = userHolder.getCurrentUser();

        LambdaUpdateWrapper<WhaleSettings> wrapper = new LambdaUpdateWrapper<>();

        wrapper.eq(WhaleSettings::getId,settingDeleteDTO.getId())
                .eq(WhaleSettings::getIsDeleted, 0)
                .set(WhaleSettings::getIsDeleted,1)
                .set(WhaleSettings::getDeletedAt,LocalDateTime.now())
                .set(WhaleSettings::getDeletedBy,currentUser.getId());

        return settingsMapper.update(wrapper) > 0;

    }
}
