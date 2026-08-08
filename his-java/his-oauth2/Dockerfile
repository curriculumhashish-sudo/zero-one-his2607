FROM amazoncorretto:17
# 指定维护者的名字
MAINTAINER 01star
# 将当前target目录下的jar包复制到docker容器中
ADD target/his-oauth2-1.0.0-SNAPSHOT.jar app.jar
# 定义JVM参数
ENV JAVA_OPTS="-Xms256m -Xmx256m"
ENV SPRING_ARGS=""
RUN echo "JAVA_OPTS=" $JAVA_OPTS
# 暴露服务端口（需配合 docker run -p 10400:10400 端口映射）
EXPOSE 10400
# 设置容器启动执行指令
CMD java $JAVA_OPTS -jar app.jar