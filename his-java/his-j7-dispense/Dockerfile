FROM amazoncorretto:17
MAINTAINER 01star
ADD target/his-j7-dispense-1.0.0-SNAPSHOT.jar app.jar
ENV JAVA_OPTS="-Xms256m -Xmx256m"
ENV SPRING_ARGS=""
<<<<<<< HEAD
RUN echo "JAVA_OPTS=" $JAVA_OPTS
CMD java $JAVA_OPTS -jar app.jar --logging.file.path=/tmp/logs/spring-boot $SPRING_ARGS
=======
CMD java $JAVA_OPTS -jar app.jar --logging.file.path=/tmp/logs/spring-boot $SPRING_ARGS
>>>>>>> 513c733e37f8c86f6581e6d4b1be4958cbc2e84b
