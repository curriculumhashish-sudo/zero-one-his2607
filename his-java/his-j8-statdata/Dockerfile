FROM amazoncorretto:17
MAINTAINER Zzen
ADD target/his-j8-statdata-1.0.0-SNAPSHOT.jar app.jar
ENV JAVA_OPTS="-Xms256m -Xmx256m"
ENV SPRING_ARGS=""
RUN echo "JAVA_OPTS=" $JAVA_OPTS
CMD java $JAVA_OPTS -jar app.jar --logging.file.path=/tmp/logs/spring-boot $SPRING_ARGS