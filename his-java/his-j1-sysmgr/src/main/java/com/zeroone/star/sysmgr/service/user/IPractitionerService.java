package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.sysmgr.entity.user.Practitioner;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 医务人员主数据 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
public interface IPractitionerService extends IService<Practitioner> {

    /**
     *
     * @param userId
     * @param userAddDTO
     */
    void add(String userId, UserAddDTO userAddDTO);
}
