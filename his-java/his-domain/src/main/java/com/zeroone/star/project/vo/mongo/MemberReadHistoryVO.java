package com.zeroone.star.project.vo.mongo;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import org.springframework.format.annotation.DateTimeFormat;

import java.util.Date;

/**
 * <p>
 * 描述：用户商品浏览历史记录数据显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "浏览记录数据传输对象")
public class MemberReadHistoryVO {
    @Schema(description = "数据唯一编号", example = "1")
    private String id;
    @Schema(description = "会员编号", example = "2")
    private Long memberId;
    @Schema(description = "会员名称", example = "王麻子")
    private String memberNickname;
    @Schema(description = "会员图标", example = "2.icon")
    private String memberIcon;
    @Schema(description = "浏览商品编号", example = "1")
    private Long productId;
    @Schema(description = "浏览商品名称", example = "洗面奶")
    private String productName;
    @Schema(description = "浏览商品图片", example = "5.png")
    private String productPic;
    @Schema(description = "浏览商品描述", example = "这是一个神奇的洗面奶")
    private String productSubTitle;
    @Schema(description = "浏览商品价格", example = "199")
    private String productPrice;
    @Schema(description = "创建时间", example = "2024-01-11 11:11:11")
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private Date createTime;
}
