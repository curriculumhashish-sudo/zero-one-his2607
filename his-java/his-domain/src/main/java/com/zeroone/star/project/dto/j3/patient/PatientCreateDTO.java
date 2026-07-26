package com.zeroone.star.project.dto.j3.patient;

import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.PastOrPresent;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：新增患者数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 照夜清
 * @version 1.0.0
 */
@Data
@Schema(description = "新增患者数据传输对象")
public class PatientCreateDTO {

    @Schema(
            description = "患者姓名",
            example = "张三",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @NotBlank(message = "患者姓名不能为空")
    @Size(max = 255, message = "患者姓名长度不能超过255个字符")
    private String name;

    @Schema(
            description = "性别编码",
            example = "MALE",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @NotBlank(message = "性别编码不能为空")
    @Pattern(regexp = "MALE|FEMALE|UNKNOWN", message = "性别编码仅支持MALE、FEMALE或UNKNOWN")
    private String genderEnum;

    @Schema(
            description = "出生日期",
            example = "1990-01-01",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @JsonDeserialize(using = FlexibleLocalDateDeserializer.class)
    @NotNull(message = "出生日期不能为空")
    @PastOrPresent(message = "出生日期不能晚于当前日期")
    private LocalDate birthDate;

    @Schema(
            description = "年龄",
            example = "36",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @NotNull(message = "年龄不能为空")
    @Min(value = 0, message = "年龄不能小于0")
    @Max(value = 150, message = "年龄不能大于150")
    private Integer age;

    @Schema(
            description = "身份证号",
            example = "110101199001011234",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @NotBlank(message = "证件号不能为空")
    @Pattern(
            regexp = "(?:\\d{15}|\\d{17}[\\dXx])",
            message = "证件号格式不正确，应为15位或18位身份证号"
    )
    @Size(max = 32, message = "身份证号长度不能超过32个字符")
    private String idCard;

    @Schema(
            description = "婚姻状态编码",
            example = "MARRIED",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String maritalStatusEnum;

    @Schema(
            description = "职业编码",
            example = "EMPLOYEE",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String prfsEnum;

    @Schema(
            description = "联系电话",
            example = "13800000000",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    @Size(max = 32, message = "联系电话长度不能超过32个字符")
    private String phone;

    @Schema(
            description = "详细地址",
            example = "北京市朝阳区示例路1号",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    @Size(max = 255, message = "详细地址长度不能超过255个字符")
    private String address;

    @Schema(
            description = "省份",
            example = "北京市",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String addressProvince;

    @Schema(
            description = "城市",
            example = "北京市",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String addressCity;

    @Schema(
            description = "区县",
            example = "朝阳区",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String addressDistrict;

    @Schema(
            description = "街道",
            example = "示例街道",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String addressStreet;

    @Schema(
            description = "国籍编码",
            example = "CHN",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String nationalityCode;

    @Schema(
            description = "ABO血型编码",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Integer bloodAbo;

    @Schema(
            description = "RH血型编码",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Integer bloodRh;

    @Schema(
            description = "工作单位",
            example = "示例科技有限公司",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String workCompany;

    @Schema(
            description = "籍贯",
            example = "北京市",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String nativePlace;

    @Schema(
            description = "国家或地区编码",
            example = "CN",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String countryCode;

    @Schema(
            description = "紧急联系人姓名",
            example = "李四",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String linkName;

    @Schema(
            description = "联系人关系编码",
            example = "1",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Integer linkRelationCode;

    @Schema(
            description = "联系人电话",
            example = "13900000000",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    @Size(max = 32, message = "联系人电话长度不能超过32个字符")
    private String linkTelcom;

    @Schema(
            description = "死亡日期",
            example = "2025-01-01 00:00:00",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    @PastOrPresent(message = "死亡日期不能晚于当前时间")
    private LocalDateTime deceasedDate;

    @Schema(
            description = "建档机构ID",
            example = "322663105141149696",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    @NotBlank(message = "建档机构ID不能为空")
    private String organizationId;
}
