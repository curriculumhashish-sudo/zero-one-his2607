package com.zeroone.star.project.j1.user;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j1.sysmgr.ChangePasswordDTO;
import com.zeroone.star.project.dto.j1.sysmgr.TenantQueryDTO;
import com.zeroone.star.project.dto.j1.sysmgr.UserDTO;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.query.j1.sysmgr.UserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserDetailVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserListVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserPageVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;

import java.util.Map;


/**
 * <p>
 * 描述：用户管理接口
 * </p>
 */
public interface UserApis {


    /**
     * 分页查询用户信息
     * @param userQuery 用户查询参数
     * @return 用户信息分页列表
     */
    JsonVO<PageDTO<UserPageVO>> getPage(UserQuery userQuery);

    /**
     * 列表用户信息
      * @param tenantQuery 用户查询参数
      * @return 用户信息列表
     */
    JsonVO<PageDTO<UserListVO>> getList(TenantQuery tenantQuery);


    /**
     * 根据用户ID获取用户信息
     * @param id 用户ID
     * @return 用户详细信息
     */
    JsonVO<UserDetailVO> getById(String id);

    /**
     * 新增用户
     * @param userDTO 用户信息
     * @return 保存结果
     */
    JsonVO<String> saveUser(UserDTO userDTO);

    /**
     * 修改用户
     * @param userDTO 用户信息
     * @return 修改结果
     */
    JsonVO<String> updateUser(UserDTO userDTO);


    /**
     * 根据用户ID删除用户信息
     * @param id 用户ID
     * @return 删除结果
     */
    JsonVO<String> deleteById(String id);

    /**
     * 修改用户密码
     * @param changePasswordDTO 包含用户ID和新密码的参数
     * @return 修改结果
     */
    JsonVO<String> changePassword(ChangePasswordDTO changePasswordDTO);

    /**
     * 启用或禁用用户
      * @param id 用户ID
     * @param status 用户状态
     * @return 启用或禁用结果
     */
    JsonVO<String> updateStatus(String id,Integer status);

    /**
     * 锁定/解锁用户
      * @param id 用户ID
      * @param lockStatus 锁定状态
      * @return 锁定/解锁结果
     */
    JsonVO<String> lockUser(String id,Integer lockStatus);

}
