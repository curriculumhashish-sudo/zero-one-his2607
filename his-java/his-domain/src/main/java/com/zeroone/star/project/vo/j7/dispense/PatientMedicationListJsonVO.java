// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\dispense\PatientMedicationListJsonVO.java
package com.zeroone.star.project.vo.j7.dispense;

import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

import java.util.List;

@Schema(description = "患者用药明细列表响应")
public class PatientMedicationListJsonVO extends JsonVO<List<PatientMedicationVO>> {
}