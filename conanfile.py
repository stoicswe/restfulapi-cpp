import os
from conans import ConanFile, tools, CMake
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import CMakeDeps
from conans.errors import ConanException

class vsPractice(ConanFile):
    name = "vs-practice"
    url = ""
    license = ""
    description = ""

    exports_sources = ("*", "!build")

    short_paths = True

    export_cmake_namespace = "vs-practice"

    settings = "os", "compiler", "build_type", "arch"

    generators = ("cmake_find_package")
    
    def requirements(self):
        self.requires("libcurl/8.0.1")
        self.requires("plog/1.1.9")
    
    #def imports(self):
    #    super().imports()
    
    def build(self):
        super().build()
        self.cmake = CMake(self)
        self.cmake.configure(build_folder=self.build_folder)
        self.cmake.build()
    
    def package(self):
        super().package()
    
    def package_info(self):
        super().package_info()
    
    def generate(self):
        dp = CMakeDeps(self)
        dp.generate()
        tc = CMakeToolchain(self)
        tc.generate()