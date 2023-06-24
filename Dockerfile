FROM ubuntu:20.04
LABEL maintainer="Debby Nirwan"
ENV DEBIAN_FRONTEND noninteractive
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# install dependencies via apt
ENV DEBCONF_NOWARNINGS yes
RUN set -x && \
  apt update -y -qq && \
  apt upgrade -y -qq --no-install-recommends && \
  : "basic dependencies" && \
  apt install -y -qq \
    build-essential \
    clang-format \
    clang-tidy \
    gcovr \
    python3-pip \
    cmake && \
  : "Google test dependencies" && \
  apt install -y -qq \
    libgtest-dev && \
  : "remove cache" && \
  apt autoremove -y -qq && \
  rm -rf /var/lib/apt/lists/*

# install dependencies via pip
RUN python3 -m pip install --no-cache-dir --upgrade pip && \
    python3 -m pip install --no-cache-dir \
                    plotly \
                    kaleido \
                    pandas \
                    "pybind11[global]"

WORKDIR /work
ENTRYPOINT ["/bin/bash"]
