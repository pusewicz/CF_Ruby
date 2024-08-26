require 'rake/clean'

# Project configuration
PROJECT_NAME = "cf_ruby"

# Directory structure
MRUBY_DIR = File.expand_path("mruby")
CUTE_DIR = File.expand_path("cute")
SRC_DIR = File.expand_path("src")
BUILD_DIR = File.expand_path("build")
LIB_DIR = File.join(BUILD_DIR, "lib")
INCLUDE_DIR = File.expand_path("include")

MRUBY_VERSION = "3.3.0"
CUTE_VERSION = "1.0.1"
CC = ENV['CC'] || 'clang'
CFLAGS = ["-std=c17", "-Wall", "-Wextra", "-pedantic", "-I./include", "-Imruby/include"]
LDFLAGS = ["-L./build/lib"]
LIBS = ["-lmruby", "-lphysfs", "-lSDL2", "-lobjc", "-lc++"]
FRAMEWORKS = %W[
  -Wl,-F#{LIB_DIR}
  -Wl,-framework,cute
  -Wl,-framework,CoreVideo
  -Wl,-framework,Cocoa
  -Wl,-framework,IOKit
  -Wl,-framework,ForceFeedback
  -Wl,-framework,Carbon
  -Wl,-framework,CoreAudio
  -Wl,-framework,AudioToolbox
  -Wl,-framework,AVFoundation
  -Wl,-framework,Foundation
  -Wl,-weak_framework,GameController
  -Wl,-weak_framework,Metal
  -Wl,-weak_framework,QuartzCore
  -Wl,-weak_framework,CoreHaptics
]

# Source and object files
SRC_FILES = FileList["#{SRC_DIR}/**/*.c"]
OBJ_FILES = SRC_FILES.pathmap("#{BUILD_DIR}/%n.o")

# Clean task
CLEAN.include(BUILD_DIR)

directory BUILD_DIR
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
      sh "cmake -Bbuild -DCF_FRAMEWORK_BUILD_SAMPLES=OFF -DCF_FRAMEWORK_BUILD_TESTS=OFF"
    end

    sh "cmake --build build"
  end

  ln_sf Dir["#{CUTE_DIR}/build/*.a"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/build/cute.framework"], LIB_DIR
  ln_sf Dir["#{CUTE_DIR}/build/_deps/sdl2-build/*.a"], LIB_DIR
end

# Compile task
rule '.o' => [->(f){source_for_object(f)}] do |t|
  mkdir_p File.dirname(t.name)
  sh "#{CC} #{CFLAGS.join(' ')} -c #{t.source} -o #{t.name}"
end

task :compile => [BUILD_DIR] + OBJ_FILES

# Link task
task :link do
  sh "#{CC} #{OBJ_FILES} #{LDFLAGS.join(' ')} #{LIBS.join(' ')} #{FRAMEWORKS.join(' ')} -o #{BUILD_DIR}/#{PROJECT_NAME}"
end

# Helper method to find source file for object file
def source_for_object(object)
  SRC_FILES.detect { |s| s.pathmap("%n") == object.pathmap("%n") }
end

# Run task
task :run => [:all] do
  sh "#{BUILD_DIR}/#{PROJECT_NAME}"
end

# Install task
task :install => [:all] do
  mkdir_p "/usr/local/bin"
  cp "#{BUILD_DIR}/#{PROJECT_NAME}", "/usr/local/bin/"
end
