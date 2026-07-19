package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j1.sysmgr.UserApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.sysmgr.UserDetailVO;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.DeleteMapping;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "用户管理")
public class UserController implements UserApis {
    /**
     * 1. 获取用户分页列表（条件查询 + 分页）
     *
     * @return
     */
    @GetMapping("/getPage")
    public JsonVO<PageDTO<UserDetailVO>> getPage() {
        return null;
    }

    /**
     * 2. 获取用户列表
     */
    @GetMapping("/getList")
    public void getList() {
    }

    /**
     * 3. 根据用户ID获取用户详细信息
     */
    @GetMapping("/getById")
    public void getById() {
    }

    /**
     * 4. 新增用户
     */
    @PostMapping("/add")
    public void add() {
    }

    /**
     * 5. 更新用户信息
     */
    @PutMapping("/update")
    public void update() {
    }

    /**
     * 6. 删除用户
     */
    @DeleteMapping("/delete")
    public void delete() {
    }

    /**
     * 7. 修改密码
     */
    @PutMapping("/changePassword")
    public void changePassword() {
    }

    /**
     * 8. 启用/禁用用户 (合并)
     */
    @PutMapping("/enableOrDisable")
    public void enableOrDisable() {
    }

    /**
     * 9. 锁定/解锁用户 (合并)
     */
    @PutMapping("/lockOrUnlock")
    public void lockOrUnlock() {
    }
}
