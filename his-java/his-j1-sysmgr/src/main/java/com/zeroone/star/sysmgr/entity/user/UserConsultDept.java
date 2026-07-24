package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import java.io.Serializable;

import lombok.*;
import lombok.experimental.Accessors;

/**
 * <p>
 * 用户-出诊科室关联表
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_user_consult_dept")
public class UserConsultDept implements Serializable {

    /**
     * 主键ID
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 用户ID
     */
    private String userId;

    /**
     * 出诊科室ID
     */
    private String deptId;


}
