# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:3f007cb3f3f0e3f9d0b7f51fa0bbec822c310bcf786023205c6385703ad60032

COPY . $SRC/VectorPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/VectorPro
