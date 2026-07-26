package com.zeroone.star.project.entity.j3;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import com.fasterxml.jackson.annotation.JsonFormat;
import com.zeroone.star.project.enums.j3.FeeCategoryEnum;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 费用性质配置表
 * </p>
 *
 * @author tangcz
 * @since 2026-07-18
 */
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@TableName("t_fee_nature")
public class FeeNature implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     */
    @TableId(type = IdType.AUTO)
    private Long id;

    /**
     * 费用性质名称
     */
    private String name;

    /**
     * 费用性质编码
     */
    private String code;

    /**
     * 显示顺序
     */
    private Integer sortOrder;

    /**
     * 分类id 0自费 1公费 2省医保 3市医保
     */
    private FeeCategoryEnum category;

    /**
     * 是否启用 1启用 0停用
     */
    private Integer isEnabled;

    /**
     * 创建时间（数据库自动填充）
     */
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "GMT+8")
    private LocalDateTime createTime;

    /**
     * 创建人
     */
    private String createBy;

    /**
     * 更新时间（数据库自动更新）
     */
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "GMT+8")
    private LocalDateTime updateTime;

    /**
     * 更新人
     */
    private String updateBy;
}