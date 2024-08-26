MRuby::Gem::Specification.new("mruby-cf") do |spec|
  sdl2_config = File.expand_path('../build/lib/sdl2-config')

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
    -Wl,-framework,AudioToolbox
    -Wl,-framework,AVFoundation
    -Wl,-framework,Foundation
    -Wl,-weak_framework,GameController
    -Wl,-weak_framework,Metal
    -Wl,-weak_framework,QuartzCore
    -Wl,-weak_framework,CoreHaptics
  ]

  linker.library_paths << File.expand_path("../build/lib")
  linker.libraries.push("physfs")
  linker.libraries.push("c++")
  linker.libraries.push("SDL2")
end
