// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\dispense\InpatientDispenseRecordPageJsonVO.java
package com.zeroone.star.project.vo.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "住院发退药记录分页响应")
public class InpatientDispenseRecordPageJsonVO extends JsonVO<PageDTO<InpatientDispenseRecordVO>> {
}