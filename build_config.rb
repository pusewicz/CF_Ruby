MRuby::Build.new do |conf|
  # load specific toolchain settings
  conf.toolchain

  # Load defaults for MRuby core
  conf.gembox 'default'

  # Your custom mrbgem
  conf.gem '../mrbgems/mruby-cf'  # Adjust the path as needed

  # C compiler settings
  conf.cc do |cc|
    cc.command = ENV['CC'] || 'clang'
  end
end
