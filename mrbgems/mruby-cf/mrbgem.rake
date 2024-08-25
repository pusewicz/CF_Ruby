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
  # cxx.include_paths << File.expand_path("../gosu/ffi")
  # cc.defines.push("GOSU_FFI_EXPORTS", "GOSU_DEPRECATED=")
  # cxx.defines.push("GOSU_FFI_EXPORTS", "GOSU_DEPRECATED=")
  # cc.flags << "/std:c++11"
  # cxx.flags << "/std:c++11"

  cc.flags << `#{sdl2_config} --cflags`.strip

  # Set Cocoa framework for Mac OS X
  linker.flags << `#{sdl2_config} --static-libs`.strip
  linker.library_paths << File.expand_path("../build/lib")
  linker.libraries.push("cute")
  linker.libraries.push("physfs")
  linker.libraries.push("c++")
  # linker.libraries.push("SDL2")
end
