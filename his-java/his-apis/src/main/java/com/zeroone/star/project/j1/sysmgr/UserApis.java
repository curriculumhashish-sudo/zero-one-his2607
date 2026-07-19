package com.zeroone.star.project.j1.sysmgr;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserDetailVO;

/**
 * <p>
 * 描述：用户管理接口
 * </p>
 */
public interface UserApis {

    /**
     * 1. 获取用户分页列表（条件查询 + 分页）
     */
    JsonVO<PageDTO<UserDetailVO>> getPage();

    /**
     * 2. 获取用户列表
     */
    public void getList();

    /**
     * 3. 根据用户ID获取用户详细信息
     */
    public void getById();

    /**
     * 4. 新增用户
     */
    public void add();

    /**
     * 5. 更新用户信息
     */
    public void update();

    /**
     * 6. 删除用户
     */
    public void delete();

    /**
     * 7. 修改密码
     */
    public void changePassword();

    /**
     * 8. 启用/禁用用户 (合并)
     */
    public void enableOrDisable();

    /**
     * 9. 锁定/解锁用户 (合并)
     */
    public void lockOrUnlock();
}
