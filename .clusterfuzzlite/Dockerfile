# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:5f1a3ea9278fb85c79de740fe6fbcbb4948fbb81e71cc1c6f21e0abc86681e8c

COPY . $SRC/VectorPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/VectorPro
