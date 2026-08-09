// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\dispense\PatientDrugItemPageJsonVO.java
package com.zeroone.star.project.vo.j7.dispense;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "患者药品项目分页响应")
public class PatientDrugItemPageJsonVO extends JsonVO<IPage<PatientDrugItemVO>> {
}