package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.query.j1.user.UserListQuery;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.sysmgr.entity.user.User;
import com.baomidou.mybatisplus.extension.service.IService;
import jakarta.validation.Valid;

/**
 * <p>
 * 系统用户 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
public interface IUserService extends IService<User> {

    /**
     * 软删除用户
     * @param id        用户ID
     * @return 是否删除成功
     */
    boolean softDeleteById(String id);

    /**
     * 修改用户启用状态
     * @param id       用户ID
     * @param isActive 是否启用 (1:启用, 0:停用)
     * @return 是否修改成功
     */
    boolean updateStatus(String id, Integer isActive);

    /**
     * 锁定/解锁用户
     * @param id          用户ID
     * @param isLockedOut 是否锁定 (1:锁定, 0:解锁)
     * @return 是否修改成功
     */
    boolean lockUser(String id, Integer isLockedOut);

    /**
     * 修改密码
     * @param id          用户ID
     * @param newPassword 新密码（明文）
     * @return 是否修改成功
     */
    boolean updatePassword(String id, String newPassword);

    /**
     * 分页查询用户列表
     * @param query 查询条件+分页
     * @return 分页结果
     */
    PageDTO<UserListVO> getList(UserListQuery query);

    /**
     * 新增用户
     * @param userAddDTO 新增用户表单
     * @return 新增用户结果
     */
    Boolean saveUser(@Valid UserAddDTO userAddDTO);
}
