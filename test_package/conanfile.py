from conans import ConanFile, CMake, tools
import os

class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = [
        "Dimensional/0.0.2@myuser/testing",
        "gtest/1.12.1"
    ]

    def configure(self):
        # Enforce C++20
        if self.settings.compiler.cppstd:
            tools.check_min_cppstd(self, "20")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["DIMENSIONAL_PrecisionType"] = "USE_DOUBLE"  # or any other type you want to test
        cmake.configure()
        cmake.build()

    def test(self):
        if not tools.cross_building(self):
            # Look for the executable in the current directory, or the bin folder
            extension = ".exe" if self.settings.os == "Windows" else ""
            bin_path = os.path.join("bin", f"StaticDimension_UnitTests{extension}")

            # If the binary isn't in 'bin', check the root of the build folder
            if not os.path.isfile(bin_path):
                bin_path = os.path.join(".", f"StaticDimension_UnitTests{extension}")

            # Check if we found the file
            if os.path.isfile(bin_path):
                self.run(bin_path, run_environment=True)
            else:
                raise ConanInvalidConfiguration("test_package executable not found")