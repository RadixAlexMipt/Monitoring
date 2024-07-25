FROM gcc:latest as build
 
 
RUN apt-get update && \
    apt-get install -y cmake
 
ADD . /app/src
 
WORKDIR /app/build
 
RUN cmake -DBUILD_SHARED_LIBS=False ../src && \
    make -j 8
 
FROM ubuntu:latest
 
RUN groupadd -r monitoring && useradd -r -g monitoring monitoring
USER monitoring
 
WORKDIR /app
 
COPY --from=build /app/src/bin /app/bin
COPY --from=build /app/src/lib /app/lib
 
RUN export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/lib/
 
ENTRYPOINT ["/app/bin/Monitoring"]