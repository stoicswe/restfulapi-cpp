import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import save, load
from conan.tools.gnu import AutotoolsToolchain, AutotoolsDeps
from conan.tools.microsoft import unix_path, VCVars, is_msvc
from conan.errors import ConanInvalidConfiguration
from conan.errors import ConanException

class restfulApi(ConanFile):
    name = "restful-api"
    url = ""
    license = ""
    description = ""

    exports_sources = ("*", "!build")

    short_paths = True

    export_cmake_namespace = "restful-api"

    settings = "os", "compiler", "build_type", "arch"

    #generators = ("cmake_find_package")
    
    def requirements(self):
        self.requires("libcurl/8.1.2")
        # Cannot seem to pull down
        #self.requires("spdlog/1.12.0")
    
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