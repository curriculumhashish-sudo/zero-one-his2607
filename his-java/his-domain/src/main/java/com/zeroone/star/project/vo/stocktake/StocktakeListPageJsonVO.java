// 文件路径：D:\01HIS\zero-one-his2607\his-java\his-domain\src\main\java\com\zeroone\star\project\vo\stocktake\StocktakeListPageJsonVO.java
package com.zeroone.star.project.vo.stocktake;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "盘点单分页列表响应")
public class StocktakeListPageJsonVO extends JsonVO<PageDTO<StocktakeListVO>> {
}