MRuby::Gem::Specification.new("mruby-cf") do |spec|
  spec.license = "Zlib"
  spec.authors = "pusewicz"
  spec.summary = "mruby bindings for the Cute Framework game library"

  cc.include_paths << File.expand_path("../mruby/include")
  cc.include_paths << File.expand_path("../cute/include")
  cc.include_paths << File.expand_path("../cute/libraries")
  cc.include_paths << File.expand_path("../cute/build/_deps/sdl2-build/include/SDL2")
  cc.include_paths << File.expand_path("../cute/build/_deps/sdl2-src/include")

  linker.flags << %W[
    -Wl,-F#{File.expand_path('../build/lib')}
    -Wl,-framework,cute
    -Wl,-framework,CoreVideo
    -Wl,-framework,Cocoa
    -Wl,-framework,IOKit
    -Wl,-framework,ForceFeedback
    -Wl,-framework,Carbon
    -Wl,-framework,CoreAudio
    -Wl,-framework,CoreMedia
    -Wl,-framework,AudioToolbox
    -Wl,-framework,AVFoundation
    -Wl,-framework,Foundation
    -Wl,-framework,UniformTypeIdentifiers
    -Wl,-weak_framework,GameController
    -Wl,-weak_framework,Metal
    -Wl,-weak_framework,QuartzCore
    -Wl,-weak_framework,CoreHaptics
  ]

  linker.library_paths << File.expand_path("../build/lib")
  linker.libraries.push("SDL3")
  linker.libraries.push("c++")
  linker.libraries.push("objc")
  linker.libraries.push("physfs")
  linker.libraries.push("glslang")
  linker.libraries.push("glslang-default-resource-limits")
  linker.libraries.push("SPIRV")
  linker.libraries.push("SPIRV-Tools")
  linker.libraries.push("SPIRV-Tools-opt")
  linker.libraries.push("spirv-cross-c")
  linker.libraries.push("spirv-cross-core")
  linker.libraries.push("spirv-cross-glsl")
  linker.libraries.push("spirv-cross-msl")
end
