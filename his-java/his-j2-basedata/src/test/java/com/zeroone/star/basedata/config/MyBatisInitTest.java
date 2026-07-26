package com.zeroone.star.basedata.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.baomidou.mybatisplus.core.MybatisConfiguration;
import com.baomidou.mybatisplus.core.metadata.TableInfoHelper;
import com.zeroone.star.basedata.entity.DictionaryConceptDO;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import org.apache.ibatis.builder.MapperBuilderAssistant;
import org.apache.ibatis.reflection.SystemMetaObject;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class MyBatisInitTest {

    private MetaObjectHandler handler;

    @BeforeEach
    void setUp() throws Exception {
        MapperBuilderAssistant assistant = new MapperBuilderAssistant(new MybatisConfiguration(), "");
        TableInfoHelper.initTableInfo(assistant, DictionaryConceptDO.class);
        UserHolder userHolder = mock(UserHolder.class);
        when(userHolder.getCurrentUser()).thenReturn(UserDTO.builder().id("operator-1").build());
        MyBatisInit myBatisInit = new MyBatisInit();
        myBatisInit.userHolder = userHolder;
        handler = myBatisInit.metaObjectHandler();
    }

    @Test
    void insertShouldFillCreationAndUpdateAuditFields() {
        DictionaryConceptDO concept = new DictionaryConceptDO();

        handler.insertFill(SystemMetaObject.forObject(concept));

        assertNotNull(concept.getCreatedAt());
        assertNotNull(concept.getUpdatedAt());
        assertEquals("operator-1", concept.getCreatedBy());
        assertEquals("operator-1", concept.getUpdatedBy());
        assertNull(concept.getDeletedAt());
    }

    @Test
    void normalUpdateShouldNotFillDeletionAuditFields() {
        DictionaryConceptDO concept = new DictionaryConceptDO();

        handler.updateFill(SystemMetaObject.forObject(concept));

        assertNotNull(concept.getUpdatedAt());
        assertEquals("operator-1", concept.getUpdatedBy());
        assertNull(concept.getDeletedAt());
        assertNull(concept.getDeletedBy());
    }

    @Test
    void logicalDeleteShouldFillUpdateAndDeletionAuditFields() {
        DictionaryConceptDO concept = new DictionaryConceptDO();
        concept.setIsDeleted(true);

        handler.updateFill(SystemMetaObject.forObject(concept));

        assertNotNull(concept.getUpdatedAt());
        assertNotNull(concept.getDeletedAt());
        assertEquals("operator-1", concept.getUpdatedBy());
        assertEquals("operator-1", concept.getDeletedBy());
    }
}
