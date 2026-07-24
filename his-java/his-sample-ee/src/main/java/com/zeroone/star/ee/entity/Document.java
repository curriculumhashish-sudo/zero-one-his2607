package com.zeroone.star.ee.entity;

import lombok.Data;
import org.dromara.easyes.annotation.IndexId;
import org.dromara.easyes.annotation.IndexName;
import org.dromara.easyes.annotation.Settings;
/**
 * <p>
 * 描述：文档数据模型
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Settings(shardsNum = 3, replicasNum = 2)
@IndexName("document")
public class Document {
    /**
     * es中的唯一id
     */
    @IndexId
    private String id;
    /**
     * 文档标题
     */
    private String title;
    /**
     * 文档内容
     */
    private String content;
}
