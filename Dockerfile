FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y --no-install-recommends cmake g++ make \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DMATRIX_ENABLE_TESTS=OFF \
    && cmake --build build --parallel 2

FROM ubuntu:24.04
WORKDIR /app
COPY --from=builder /app/build/matrix_calculator /app/matrix_calculator
ENTRYPOINT ["/app/matrix_calculator"]
