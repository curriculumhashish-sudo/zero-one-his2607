// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\j7\pharmacy\PurchaseInboundListPageJsonVO.java
package com.zeroone.star.project.vo.j7.pharmacy;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "入库单分页列表响应")
public class PurchaseInboundListPageJsonVO extends JsonVO<IPage<PurchaseInboundListVO>> {
}