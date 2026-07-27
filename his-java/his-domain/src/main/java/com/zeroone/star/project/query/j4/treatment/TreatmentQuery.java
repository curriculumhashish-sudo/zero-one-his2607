package com.zeroone.star.project.query.j4.treatment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 诊疗项目分页查询对象
 *
 * 【前端对接说明】
 * 1. 本对象用于listTreatmentOptions和queryTreatmentList两个分页查询接口的入参
 * 2. 继承PageQuery，包含pageNum(页码，默认1)和pageSize(每页条数，默认10)两个分页参数
 * 3. keyword为模糊查询字段，匹配name(名称)/busNo(业务编码)/pyStr(拼音码)/wbStr(五笔码)
 * 4. categoryCode为精确筛选字段，传入编码值即可筛选对应分类，不传则查全部
 * 5. categoryCode编码对照表与VO中保持一致：21=治疗/22=检验/23=检查/24=手术等
 */
@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "诊疗项目分页查询对象")
public class TreatmentQuery extends PageQuery {

    @Size(max = 100, message = "关键词长度不能超过100个字符")
    @Schema(description = "关键词（诊疗名称/编码/拼音码/五笔码，模糊查询）", example = "CT")
    private String keyword;

    /**
     * 目录分类编码筛选，传入对应编码精确匹配
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=255）：
     * 21=治疗, 22=检验, 23=检查, 24=手术, 25=麻醉, 26=护理级别, 27=膳食, 28=输血, 29=输氧,
     * 31=其他, 32=转科, 33=术后, 34=出院, 35=转院, 36=死亡, 37=产后, 38=病情, 39=护理常规,
     * 40=体位, 41=陪护, 42=隔离等级, 43=口腔检查, 44=内镜检查, 45=内镜治疗, 46=基础服务,
     * 47=诊断费, 48=预防接种手续费, 49=口腔治疗, 50=护理项目, 51=床位服务
     */
    @Schema(description = "目录分类编码，精确筛选：21=治疗/22=检验/23=检查/24=手术/25=麻醉/26=护理级别/27=膳食/28=输血/29=输氧/31=其他/32=转科/33=术后/34=出院/35=转院/36=死亡/37=产后/38=病情/39=护理常规/40=体位/41=陪护/42=隔离等级/43=口腔检查/44=内镜检查/45=内镜治疗/46=基础服务/47=诊断费/48=预防接种手续费/49=口腔治疗/50=护理项目/51=床位服务", example = "22")
    private String categoryCode;
}
