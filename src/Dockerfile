FROM quay.io/school21/alpine:3.16 as builder

RUN : \
    && apk --no-cache --upgrade add \
        build-base \
        git \
        git-lfs \
        curl \
        jq \
        make \
        bash \
        python3 \
        valgrind \
        cppcheck \
        alpine-sdk \
        pcre-dev \
        clang-extra-tools \
        check \
        check-dev \
    && :
