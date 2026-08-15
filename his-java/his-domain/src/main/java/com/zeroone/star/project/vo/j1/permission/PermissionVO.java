package com.zeroone.star.project.vo.j1.permission;

import lombok.Data;
import java.io.Serializable;
import java.time.LocalDateTime;

@Data
public class PermissionVO implements Serializable {
    private String id;
    private String name;
    private String displayName;
    private String parentName;
    private String description;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
}