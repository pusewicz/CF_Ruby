require 'rake/clean'

# Project configuration
PROJECT_NAME = "cf_ruby"

# Directory structure
MRUBY_DIR = File.expand_path("mruby")
CUTE_DIR = File.expand_path("cute")
SRC_DIR = File.expand_path("src")
BUILD_DIR = File.expand_path("build")
BIN_DIR = File.join(BUILD_DIR, "bin")
LIB_DIR = File.join(BUILD_DIR, "lib")
INCLUDE_DIR = File.expand_path("include")

EXE_PATH = File.join(BIN_DIR, PROJECT_NAME)

MRUBY_VERSION = "3.3.0"
CUTE_VERSION = "master"
CC = ENV['CC'] || 'clang'
CFLAGS = ["-std=c17", "-Wall", "-Wextra", "-pedantic", "-I./include", "-I./include/SDL3"]
LDFLAGS = ["-L./build/lib"]
LIBS = %W[
  -lmruby
  -lphysfs
  -lSDL3
  -lobjc
  -lc++
  -lglslang
  -lglslang-default-resource-limits
  -lSPIRV
  -lSPIRV-Tools
  -lSPIRV-Tools-opt
  -lspirv-cross-c
  -lspirv-cross-core
  -lspirv-cross-glsl
  -lspirv-cross-msl
  -Wl,-F#{LIB_DIR}
  -Wl,-framework,CoreVideo
  -Wl,-framework,CoreMedia
  -Wl,-framework,Cocoa
  -Wl,-framework,IOKit
  -Wl,-framework,ForceFeedback
  -Wl,-framework,Carbon
  -Wl,-framework,CoreAudio
  -Wl,-framework,AudioToolbox
  -Wl,-framework,AVFoundation
  -Wl,-framework,Foundation
  -Wl,-framework,UniformTypeIdentifiers
  -Wl,-weak_framework,GameController
  -Wl,-weak_framework,Metal
  -Wl,-weak_framework,QuartzCore
  -Wl,-weak_framework,CoreHaptics
  -Wl,-framework,cute
]
# Source and object files
SRC_FILES = FileList["#{SRC_DIR}/**/*.c"]
OBJ_FILES = SRC_FILES.pathmap("#{BUILD_DIR}/%n.o")

# Clean task
CLEAN.include(BUILD_DIR)
CLOBBER.include(MRUBY_DIR)
CLOBBER.include(CUTE_DIR)

directory BUILD_DIR
directory BIN_DIR
directory LIB_DIR
directory INCLUDE_DIR

# Default task
task :default => [:all]

# Build task
task :all => [:cute, :mruby, :compile, :link]

# MRuby tasks
task :mruby => [LIB_DIR, INCLUDE_DIR] do
  unless Dir.exist?(MRUBY_DIR)
    sh "git clone --depth 1 --single-branch --branch #{MRUBY_VERSION} https://github.com/mruby/mruby.git #{MRUBY_DIR}"
  end

  ln_sf File.expand_path("build_config.rb"), MRUBY_DIR + "/build_config/cf_ruby.rb"

  Dir.chdir(MRUBY_DIR) do
    sh "rake", "MRUBY_CONFIG=cf_ruby"
  end

  ln_sf Dir["#{MRUBY_DIR}/build/host/lib/*.a"], LIB_DIR
  ln_sf Dir["#{MRUBY_DIR}/include/*"], INCLUDE_DIR
end

task :cute => [LIB_DIR, INCLUDE_DIR] do
  unless Dir.exist?(CUTE_DIR)
    sh "git clone --depth 1 --single-branch --branch #{CUTE_VERSION} https://github.com/RandyGaul/cute_framework.git #{CUTE_DIR}"
  end

  # Checkout the correct version of Cute Framework
  sh "git -C #{CUTE_DIR} checkout #{CUTE_VERSION}"

  Dir.chdir(CUTE_DIR) do
    unless Dir.exist?("build")
      mkdir_p "build"
      sh "cmake -GNinja -Bbuild -DCF_FRAMEWORK_BUILD_SAMPLES=OFF -DCF_FRAMEWORK_BUILD_TESTS=OFF"
    end
    sh "cmake --build build"
  end

  ln_sf Dir["#{CUTE_DIR}/build/*.a"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/build/cute.framework"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/build/_deps/sdl3-build/*.a"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/build/_deps/spirv_cross-build/*.a"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/include/*"], INCLUDE_DIR
  ln_sf Dir["#{CUTE_DIR}/libraries/*"], INCLUDE_DIR
  ln_sf Dir["#{CUTE_DIR}/build/_deps/sdl3-build/include/*"], INCLUDE_DIR
  ln_sf Dir["#{CUTE_DIR}/build/_deps/sdl3-build/include-config-release/SDL3/*"], INCLUDE_DIR + "/SDL3"
end

# Compile task
rule '.o' => [->(f){source_for_object(f)}] do |t|
  mkdir_p File.dirname(t.name)
  sh "#{CC} #{CFLAGS.join(' ')} -c #{t.source} -o #{t.name}"
end

task :compile => [BUILD_DIR] + OBJ_FILES

# Link task
task :link => [BIN_DIR] do
  sh "#{CC} #{OBJ_FILES} #{LDFLAGS.join(' ')} #{LIBS.join(' ')} -o #{EXE_PATH}"
end

# Helper method to find source file for object file
def source_for_object(object)
  SRC_FILES.detect { |s| s.pathmap("%n") == object.pathmap("%n") }
end

# Run task
task :run => [:all] do
  cp "#{SRC_DIR}/main.rb", BUILD_DIR
  Dir.chdir(BUILD_DIR) do
    sh EXE_PATH
  end
end

# Install task
task :install => [:all] do
  mkdir_p "/usr/local/bin"
  cp "#{BUILD_DIR}/#{PROJECT_NAME}", "/usr/local/bin/"
end
