FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN g++ -O3 main.cpp -o server -lssl -lcrypto -lpthread

EXPOSE 8080

CMD ["./server"]
