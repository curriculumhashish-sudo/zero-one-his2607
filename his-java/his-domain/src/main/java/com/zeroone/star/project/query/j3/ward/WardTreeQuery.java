package com.zeroone.star.project.query.j3.ward;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonToken;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：根据条件机构名称获取病区/病房/床位列表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhu
 * @version 1.0.0
 */

@Data
@Schema(description = "病区-病房-床位查询条件")
public class WardTreeQuery extends PageQuery {
    @Schema(description = "唯一标识", example = "322663218269917184")
    private String id;

    @Schema(description = "名称（支持模糊匹配）", example = "内科疗区")
    private String name;

    @Schema(description = "机构名称", example = "住院内科")
    private String orgName;

    @Schema(description = "子节点列表")
    @JsonDeserialize(using = WardTreeQueryListDeserializer.class)
    private List<WardTreeQuery> children;

    /**
     * 自定义反序列化器，兼容前端传入 children: true / false / null / 数组的情况。
     * 前端可能误将 children 传为布尔值，此时将其忽略为 null。
     */
    public static class WardTreeQueryListDeserializer extends JsonDeserializer<List<WardTreeQuery>> {
        @Override
        public List<WardTreeQuery> deserialize(JsonParser p, DeserializationContext ctxt) throws IOException {
            JsonToken token = p.currentToken();
            if (token == JsonToken.START_ARRAY) {
                List<WardTreeQuery> result = new ArrayList<>();
                while (p.nextToken() != JsonToken.END_ARRAY) {
                    result.add(ctxt.readValue(p, WardTreeQuery.class));
                }
                return result;
            }
            // 兼容前端可能的布尔值或 null，直接忽略
            return null;
        }
    }
}
