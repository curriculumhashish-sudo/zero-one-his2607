package com.zeroone.star.configuration.orderset.mapper;

import org.apache.ibatis.builder.xml.XMLMapperBuilder;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.Configuration;
import org.junit.jupiter.api.Test;

import java.io.InputStream;

import static org.junit.jupiter.api.Assertions.assertTrue;

class OrderSetMapperXmlTest {

    @Test
    void mapperXmlFilesShouldBeLoadableByMyBatis() throws Exception {
        Configuration configuration = new Configuration();
        parse(configuration, "mapper/orderset/OrderSetMapper.xml");
        parse(configuration, "mapper/orderset/OrderSetLineMapper.xml");

        assertTrue(configuration.hasStatement(OrderSetMapper.class.getName()
                + ".updateContentByVersion"));
        assertTrue(configuration.hasStatement(OrderSetLineMapper.class.getName()
                + ".insertBatch"));
        assertTrue(configuration.hasStatement(OrderSetLineMapper.class.getName()
                + ".softDeleteByIds"));
    }

    private void parse(Configuration configuration, String resource) throws Exception {
        try (InputStream inputStream = Resources.getResourceAsStream(resource)) {
            new XMLMapperBuilder(inputStream, configuration, resource,
                    configuration.getSqlFragments()).parse();
        }
    }
}
