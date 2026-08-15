package com.zeroone.star.project.j1.user;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j1.user.ChangePasswordDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.dto.j1.user.UserUpdateDTO;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.query.j1.user.UserListQuery;
import com.zeroone.star.project.query.j1.user.UserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.user.UserDetailVO;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.project.vo.j1.user.UserPageVO;


/**
 * <p>
 * 描述：用户管理接口
 * </p>
 */
public interface UserApis {
    /**
     * 分页查询用户信息
     * @param query 用户查询参数
     * @return 用户信息分页列表
     */
    JsonVO<PageDTO<UserPageVO>> getPage(UserQuery query);

    /**
     * 列表用户信息
      * @param query 用户查询参数
      * @return 用户信息列表
     */
    JsonVO<PageDTO<UserListVO>> getList(UserListQuery query);


    /**
     * 根据用户ID获取用户信息
     * @param id 用户ID
     * @return 用户详细信息
     */
    JsonVO<UserDetailVO> getById(String id);

    /**
     * 新增用户
     * @param userAddDTO 用户信息
     * @return 保存结果
     */
    JsonVO<Boolean> saveUser(UserAddDTO userAddDTO);

    /**
     * 修改用户
     * @param userUpdateDTO 用户信息
     * @return 修改结果
     */
    JsonVO<Boolean> updateUser(UserUpdateDTO userUpdateDTO);


    /**
     * 根据用户ID删除用户信息
     * @param id 用户ID
     * @return 删除结果
     */
    JsonVO<Boolean> deleteById(String id);

    /**
     * 修改用户密码
     * @param changePasswordDTO 包含用户ID和新密码的参数
     * @return 修改结果
     */
    JsonVO<Boolean> changePassword(ChangePasswordDTO changePasswordDTO);

    /**
     * 启用或禁用用户
      * @param id 用户ID
     * @param status 用户状态
     * @return 启用或禁用结果
     */
    JsonVO<Boolean> updateStatus(String id,Integer status);

    /**
     * 锁定/解锁用户
      * @param id 用户ID
      * @param lockStatus 锁定状态
      * @return 锁定/解锁结果
     */
    JsonVO<Boolean> lockUser(String id,Integer lockStatus);

}
