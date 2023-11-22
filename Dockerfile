# syntax=docker/dockerfile-upstream:master-labs

FROM emscripten/emsdk AS emsdk-base
ENV INSTALL_DIR=/opt
ENV CFLAGS="-I$INSTALL_DIR/include $CFLAGS $EXTRA_CFLAGS"
ENV CXXFLAGS="$CFLAGS"
ENV EM_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

# Build zlib
FROM emsdk-base AS zlib-builder
ENV ZLIB_BRANCH=v1.2.11

ADD https://github.com/ffmpegwasm/zlib.git#$ZLIB_BRANCH /src
COPY build/zlib.sh /src/build.sh
RUN bash -x /src/build.sh

# Build libwebp
FROM emsdk-base AS libwebp-builder
COPY --from=zlib-builder $INSTALL_DIR $INSTALL_DIR
ENV LIBWEBP_BRANCH=v1.1.0
ADD https://github.com/ffmpegwasm/libwebp.git#$LIBWEBP_BRANCH /src
COPY build/libwebp.sh /src/build.sh
RUN bash -x /src/build.sh


FROM emsdk-base as builder
WORKDIR /app
COPY --from=libwebp-builder $INSTALL_DIR $INSTALL_DIR
COPY src .
RUN mkdir dist && \
  em++ --bind main.cpp -o dist/webp.js \
  # Optimizations
  -O3 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MODULARIZE=1 \
  -s FORCE_FILESYSTEM=1 \
  -I$INSTALL_DIR/include \
  -L$INSTALL_DIR/lib \
  -lwebp \
  -s EXPORT_ES6=1

FROM scratch
COPY --from=builder /app/dist /