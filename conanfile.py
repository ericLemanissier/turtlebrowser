import os
from conans import ConanFile, CMake

# Conan update 68
class ConanDependencies(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    platform_qt = os.getenv("CMAKE_PREFIX_PATH")
    generators = "qt", "virtualrunenv", "cmake", "cmake_find_package_multi", "cmake_find_package", "pkg_config", "qmake" if not platform_qt else "cmake"
    
    default_options = {
        "qt:shared": True,
        "qt:qtlocation": True,
#        "qt:qtquickcontrols": True,
#        "qt:qtquickcontrols2": True,
        "qt:qttools": True,
        "qt:qtsvg": True,
        "qt:qtwebchannel": True,
        "qt:qtwebengine": True,
        "qt:qtwebview": True,
        #"qt:with_fontconfig": True,
        "qt:with_freetype": True,
        "qt:with_glib": False,
#        "qt:config": "-no-feature-geoservices_mapboxgl",
        "fontconfig:shared": True,
        #"harfbuzz:with_glib": False,
        "qt:qtshadertools":  True,
        "qt:qtwebsockets": True,
    }

    def requirements(self):
        platform_qt = os.getenv("CMAKE_PREFIX_PATH")
        if not platform_qt:
            self.output.info("CMAKE_PREFIX_PATH not set")
            self.output.info("To use the Qt from your system, set the CMAKE_PREFIX_PATH env var")
            self.output.info("Trying to get Qt from Conan")
            self.requires("qt/6.2.1")
        else:
            self.output.info("Getting Qt from the system. CMAKE_PREFIX_PATH = " + platform_qt)

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='lib', src='lib')
        self.copy('*', dst='libexec', src='libexec')
        self.copy('*', dst='bin/archdatadir/plugins', src='bin/archdatadir/plugins')
        self.copy('*', dst='bin/archdatadir/qml', src='bin/archdatadir/qml')
        self.copy('*', dst='bin/archdatadir/libexec', src='bin/archdatadir/libexec')
        self.copy('*', dst='bin/datadir/translations', src='bin/datadir/translations')
        self.copy('*', dst='resources', src='resources')
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)
