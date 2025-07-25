from conans import ConanFile, CMake
import os

class DimensionalConan(ConanFile):
    name = "Dimensional"
    version = "3.0.0.0" # Extra .0 because beta
    exports_sources = "Dimension/*"
    
    # Specify that it's a header-only library
    no_copy_source = True

    def package(self):
        self.copy("*.h", src="Dimension", dst="include", excludes=("ExampleExtensions/*", "UnitTest/*", "PhysicsProblemsExamples/*"))  

    def package_info(self):
        self.info.header_only()  # Indicate this is a header-only library
        self.cpp_info.cxxflags = ["-DUSE_DOUBLE", "-DUSE_CONAN=ON"]
    