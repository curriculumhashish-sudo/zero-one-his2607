package com.zeroone.star.project.vo.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "(用户管理)用户列表")
public class UserPageVO {
    @Schema(description = "主键id", example = "336396155648217088")
    private String id;

    @Schema(description = "用户名", example = "lianglaoshi")
    private String username;

    @Schema(description = "名")
    private String name;

    @Schema(description = "角色名称列表")
    private List<String> roleNames;

    @Schema(description = "角色id列表")
    private List<String> roleIds;


    @Schema(description = "手机号码", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "电子邮箱", example = "lianglaoshi@hospital.com")
    private String email;

    @Schema(description = "用户状态 (1:正常, 0:停用)", example = "1")
    private Integer isActive;

    @Schema(description = "锁定状态 (1:已锁定, 0:未锁定)", example = "0")
    private Integer isLockedOut;

    @Schema(description = "登录失败次数", example = "0")
    private Integer accessFailedCount;

    @Schema(description = "锁定结束时间", example = "2024-05-20 10:00:00")
    private LocalDateTime lockoutEnd;

    @Schema(description = "创建时间", example = "2023-10-27 10:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间", example = "2023-10-28 12:00:00")
    private LocalDateTime updatedAt;
}
