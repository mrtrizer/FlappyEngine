
#V8JSEngine tests
pushd modules/V8JSEngine/tests
flappy gen cmake
popd

#OggAudioRes build
pushd modules/OggAudioRes/demo
flappy gen cmake
popd

#Sdl2Manager build
pushd modules/Sdl2Manager/demo
flappy gen cmake
popd

#Core tests
pushd modules/Core/tests
flappy gen cmake
popd

#Std tests
pushd modules/Std/tests
flappy gen cmake
popd

#ResManager tests
pushd modules/ResManager/tests
flappy gen cmake
popd

#Tools test project (build only)
pushd modules/Tools/tests
flappy gen cmake
popd

#EventBus tests
pushd modules/EventBus/tests
flappy gen cmake
popd

#CoreComponents tests
pushd modules/CoreComponents/tests
flappy gen cmake
popd

#CoreResources tests
pushd modules/CoreResources/tests
flappy gen cmake
popd

#JsonRes tests
pushd modules/JsonRes/tests
flappy gen cmake
popd

#Box2DManager tests
pushd modules/Box2DManager/demo
flappy gen cmake
popd

#UI tests
pushd modules/UI/tests
flappy gen cmake
popd

#DebugDraw tests
pushd modules/DebugDraw/demo
flappy gen cmake
popd
