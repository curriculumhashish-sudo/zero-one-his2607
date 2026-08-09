// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\pharmacy\InStockOrderSaveJsonVO.java
package com.zeroone.star.project.vo.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderSaveDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "入库单新增保存响应")
public class InStockOrderSaveJsonVO extends JsonVO<InStockOrderSaveDTO> {
}