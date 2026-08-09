// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\dispense\InpatientRefundPatientPageJsonVO.java
package com.zeroone.star.project.vo.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "住院退药患者队列分页响应")
public class InpatientRefundPatientPageJsonVO extends JsonVO<PageDTO<InpatientRefundPatientVO>> {
}