node {
    docker_cmake_build_with_tests('gcc')
    docker_cmake_build_with_tests('clang')
}

def docker_cmake_build_with_tests(compiler) {
    stage(compiler) {
        node {
            checkout scm

            docker_image(compiler).inside {
                dir('build') {
                    cmake_generate_build_with_tests()
                    cmake_build()
                    execute_tests()
                }
            }
        }
    }
}

def cmake_generate_build_with_tests() {
    cmake_generate_build('-DWITH_TESTS=1 -DCMAKE_BUILD_TYPE=Release')
}

def cmake_generate_build(flags) {
    execute_command_line('cmake ' + flags + ' ..')
}

def cmake_build(flags = '') {
    execute_command_line('cmake --build . ' + flags)
}

def execute_tests() {
    execute_command_line('ctest')
}

def execute_command_line(what) {
    sh what
}

def docker_image(compiler) {
    return docker.build(compiler, './docker/' + compiler)
}
