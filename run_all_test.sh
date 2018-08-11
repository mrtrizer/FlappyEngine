export OLD_CXX=$CXX
export CXX="ccache c++"

#Reflection tests
pushd modules/Reflection/tests
flappy build cmake
pushd generated/cmake/build
./tests_Reflection
popd
popd

#MemoryManager tests
pushd modules/MemoryManager/tests
flappy build cmake
pushd generated/cmake/build
./tests_MemoryManager
popd
popd

#V8JSEngine tests
pushd modules/V8JSEngine/tests
flappy build cmake
pushd generated/cmake/build
./tests_V8JSEngine
popd
popd

#OggAudioRes build
pushd modules/OggAudioRes/demo
flappy build cmake
popd

#Sdl2Manager build
pushd modules/Sdl2Manager/demo
flappy build cmake
popd

#Core tests
pushd modules/Core/tests
flappy build cmake
pushd generated/cmake/build
./tests_Core
popd
popd

#Std tests
pushd modules/Std/tests
flappy build cmake
pushd generated/cmake/build
./tests_Std
popd
popd

#ResManager tests
pushd modules/ResManager/tests
flappy build cmake
pushd generated/cmake/build
./tests_ResManager
popd
popd

#Tools test project (build only)
pushd modules/Tools/tests
flappy build cmake
pushd generated/cmake/build
./tests_Tools
popd
popd

#EventBus tests
pushd modules/EventBus/tests
flappy build cmake
pushd generated/cmake/build
./tests_EventBus
popd
popd

#CoreComponents tests
pushd modules/CoreComponents/tests
flappy build cmake
pushd generated/cmake/build
./tests_CoreComponents
popd
popd

#CoreResources tests
pushd modules/CoreResources/tests
flappy build cmake
pushd generated/cmake/build
./tests_CoreResources
popd
popd

#JsonRes tests
pushd modules/JsonRes/tests
flappy build cmake
pushd generated/cmake/build
./tests_JsonRes
popd
popd

#Box2DManager tests
pushd modules/Box2DManager/demo
flappy build cmake
popd

#UI tests
pushd modules/UI/tests
flappy build cmake
pushd generated/cmake/build
./tests_UI
popd
popd

#DebugDraw tests
pushd modules/DebugDraw/demo
flappy build cmake
popd

export CXX=$OLD_CXX
