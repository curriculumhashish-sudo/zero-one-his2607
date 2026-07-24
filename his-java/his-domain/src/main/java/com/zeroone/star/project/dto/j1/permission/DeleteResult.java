package com.zeroone.star.project.dto.j1.permission;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class DeleteResult {
    private Boolean success;
    private Integer deletedCount;
}