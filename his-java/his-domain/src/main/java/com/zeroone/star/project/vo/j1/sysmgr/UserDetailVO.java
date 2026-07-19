package com.zeroone.star.project.vo.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "用户详细信息")
public class UserDetailVO {
    @Schema(description = "主键ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "用户名", example = "lianglaoshi", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    @Schema(description = "姓名", example = "梁老师", requiredMode = Schema.RequiredMode.REQUIRED)
    private String realName;

    @Schema(description = "角色名称", example = "医生")
    private List<String> roleNames;

    @Schema(description = "手机号码", example = "13800138000")
    private String phone;

    @Schema(description = "电子邮箱", example = "lianglaoshi@hospital.com")
    private String email;

    @Schema(description = "用户状态 (1:启用, 0:停用)", example = "1")
    private Integer status;

    @Schema(description = "锁定状态 (1:已锁定, 0:未锁定)", example = "0")
    private Integer lockStatus;

    @Schema(description = "登录失败次数", example = "0")
    private Integer loginFailCount;

    @Schema(description = "锁定结束时间", example = "2024-05-20 10:00:00")
    private LocalDateTime lockEndTime;

    @Schema(description = "创建时间", example = "2023-10-27 10:00:00")
    private LocalDateTime createTime;

    @Schema(description = "更新时间", example = "2023-10-28 12:00:00")
    private LocalDateTime updateTime;
}