from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy, get
import os

class VectorProConan(ConanFile):
    name = "vectorpro"
    version = "1.0.0"

    package_type = "header-library"

    license = "MIT"
    author = "privateMwb"
    url = "https://github.com/privateMwb/VectorPro"
    description = "High-performance std::vector-compatible contiguous container."
    topics = (
        "vector",
        "container",
        "header-only",
        "cpp",
        "data-structure"
    )

    settings = "os", "compiler", "build_type", "arch"

    no_copy_source = True

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 23)

    def source(self):
        get(
            self,
            **self.conan_data["sources"][self.version],
            strip_root=True,
        )

    def package(self):
        copy(
            self,
            "*.h",
            src=os.path.join(self.source_folder, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )
        copy(
            self,
            "*.hpp",
            src=os.path.join(self.source_folder, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )
        copy(
            self,
            "*.tpp",
            src=os.path.join(self.source_folder, "include"),
            dst=os.path.join(self.package_folder, "include"),
        )
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()
