FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    wget \
    curl \
    unzip \
    software-properties-common \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 17 \
    && apt-get install -y \
    gcc-12 \
    g++-12 \
    cmake \
    python3-pip \
    python3.12-venv \
    clang-tidy \
    && apt-get clean

# Set GCC 12 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 12

# Set Clang 17 as the default clang and clang++
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-17 17 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-17 17

# Create virtual environment and install Conan in a single command without sourcing
RUN python3 -m venv ~/conan-env && \
    ~/conan-env/bin/pip install --upgrade pip && \
    ~/conan-env/bin/pip install conan==1.65.0 && \

    ~/conan-env/bin/conan profile new gcc_debug --detect || true && \
    ~/conan-env/bin/conan profile update settings.build_type=Debug gcc_debug && \
    ~/conan-env/bin/conan profile update settings.compiler=gcc gcc_debug && \
    ~/conan-env/bin/conan profile update settings.compiler.version=12 gcc_debug && \

    ~/conan-env/bin/conan profile new gcc_release --detect || true && \
    ~/conan-env/bin/conan profile update settings.build_type=Release gcc_release && \
    ~/conan-env/bin/conan profile update settings.compiler=gcc gcc_release && \
    ~/conan-env/bin/conan profile update settings.compiler.version=12 gcc_release && \

    ~/conan-env/bin/conan profile new clang_debug --detect || true && \
    ~/conan-env/bin/conan profile update settings.build_type=Debug clang_debug && \
    ~/conan-env/bin/conan profile update settings.compiler=clang clang_debug && \
    ~/conan-env/bin/conan profile update settings.compiler.version=17 clang_debug && \

    ~/conan-env/bin/conan profile new clang_release --detect || true && \
    ~/conan-env/bin/conan profile update settings.build_type=Release clang_release && \
    ~/conan-env/bin/conan profile update settings.compiler=clang clang_release && \
    ~/conan-env/bin/conan profile update settings.compiler.version=17 clang_release

# Set up an environment variable to use the virtual environment Python and pip
ENV PATH="/root/conan-env/bin:$PATH"

# Verify installation (using the full path to the virtual environment)
RUN gcc --version && clang --version && cmake --version && ~/conan-env/bin/conan --version

RUN curl -sSLo ./sonar-scanner.zip 'https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.2.1.4610-linux-x64.zip' && \
    unzip -o sonar-scanner.zip && \
    mv sonar-scanner-6.2.1.4610-linux-x64 sonar-scanner && \
    curl -sSLo ./build-wrapper-linux-x86.zip "https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip" && \
    unzip -oj build-wrapper-linux-x86.zip -d ./build-wrapper

WORKDIR /workspace