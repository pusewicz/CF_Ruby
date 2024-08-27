def update
  @sprite ||= Cute::Sprite.new
  @sprite.play('spin') unless @sprite.playing?('spin')
  @sprite.update
  @sprite.draw
end
