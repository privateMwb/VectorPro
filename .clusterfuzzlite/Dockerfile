# ============================================================
# .clusterfuzzlite/Dockerfile
# Base image for building the fuzz harnesses under ClusterFuzzLite.
# ============================================================

FROM gcr.io/oss-fuzz-base/base-builder@sha256:14b5fea57004f2f4edcfa212aa8c77397244f307a37eb216277ac40ec0155b1e

COPY . $SRC/VectorPro
COPY .clusterfuzzlite/build.sh $SRC/

WORKDIR $SRC/VectorPro
