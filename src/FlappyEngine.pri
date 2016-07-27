INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/box2d/Box2D

HEADERS += \
    $$PWD/gl/gltools.h \
    $$PWD/gl/glshaderprogram.h \
    $$PWD/gl/glattribarray.h \
    $$PWD/gl/gltexture.h \
    $$PWD/gl/glviewfactory.h \
    $$PWD/gl/glviewshape.h \
    $$PWD/gl/glviewsprite.h \
    $$PWD/gl/glview.h \
    $$PWD/glut/glutmgr.h \
    $$PWD/qt/viewfactoryqt.h \
    $$PWD/core/flappyapp.h \
    $$PWD/core/animation.h \
    $$PWD/core/prefab.h \
    $$PWD/core/manager.h \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2ChainShape.h \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2CircleShape.h \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2EdgeShape.h \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2PolygonShape.h \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2Shape.h \
    $$PWD/box2d/Box2D/Box2D/Collision/b2BroadPhase.h \
    $$PWD/box2d/Box2D/Box2D/Collision/b2Collision.h \
    $$PWD/box2d/Box2D/Box2D/Collision/b2Distance.h \
    $$PWD/box2d/Box2D/Box2D/Collision/b2DynamicTree.h \
    $$PWD/box2d/Box2D/Box2D/Collision/b2TimeOfImpact.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2BlockAllocator.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2Draw.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2GrowableStack.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2Math.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2Settings.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2StackAllocator.h \
    $$PWD/box2d/Box2D/Box2D/Common/b2Timer.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2Contact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2GearJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2Joint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Body.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2ContactManager.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Fixture.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Island.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2TimeStep.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2World.h \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2WorldCallbacks.h \
    $$PWD/box2d/Box2D/Box2D/Rope/b2Rope.h \
    $$PWD/box2d/Box2D/Box2D/Box2D.h \
    $$PWD/glm/detail/_features.hpp \
    $$PWD/glm/detail/_fixes.hpp \
    $$PWD/glm/detail/_noise.hpp \
    $$PWD/glm/detail/_swizzle.hpp \
    $$PWD/glm/detail/_swizzle_func.hpp \
    $$PWD/glm/detail/_vectorize.hpp \
    $$PWD/glm/detail/func_common.hpp \
    $$PWD/glm/detail/func_exponential.hpp \
    $$PWD/glm/detail/func_geometric.hpp \
    $$PWD/glm/detail/func_integer.hpp \
    $$PWD/glm/detail/func_matrix.hpp \
    $$PWD/glm/detail/func_packing.hpp \
    $$PWD/glm/detail/func_trigonometric.hpp \
    $$PWD/glm/detail/func_vector_relational.hpp \
    $$PWD/glm/detail/intrinsic_common.hpp \
    $$PWD/glm/detail/intrinsic_exponential.hpp \
    $$PWD/glm/detail/intrinsic_geometric.hpp \
    $$PWD/glm/detail/intrinsic_integer.hpp \
    $$PWD/glm/detail/intrinsic_matrix.hpp \
    $$PWD/glm/detail/intrinsic_trigonometric.hpp \
    $$PWD/glm/detail/intrinsic_vector_relational.hpp \
    $$PWD/glm/detail/precision.hpp \
    $$PWD/glm/detail/setup.hpp \
    $$PWD/glm/detail/type_float.hpp \
    $$PWD/glm/detail/type_gentype.hpp \
    $$PWD/glm/detail/type_half.hpp \
    $$PWD/glm/detail/type_int.hpp \
    $$PWD/glm/detail/type_mat.hpp \
    $$PWD/glm/detail/type_mat2x2.hpp \
    $$PWD/glm/detail/type_mat2x3.hpp \
    $$PWD/glm/detail/type_mat2x4.hpp \
    $$PWD/glm/detail/type_mat3x2.hpp \
    $$PWD/glm/detail/type_mat3x3.hpp \
    $$PWD/glm/detail/type_mat3x4.hpp \
    $$PWD/glm/detail/type_mat4x2.hpp \
    $$PWD/glm/detail/type_mat4x3.hpp \
    $$PWD/glm/detail/type_mat4x4.hpp \
    $$PWD/glm/detail/type_vec.hpp \
    $$PWD/glm/detail/type_vec1.hpp \
    $$PWD/glm/detail/type_vec2.hpp \
    $$PWD/glm/detail/type_vec3.hpp \
    $$PWD/glm/detail/type_vec4.hpp \
    $$PWD/glm/gtc/bitfield.hpp \
    $$PWD/glm/gtc/color_space.hpp \
    $$PWD/glm/gtc/constants.hpp \
    $$PWD/glm/gtc/epsilon.hpp \
    $$PWD/glm/gtc/integer.hpp \
    $$PWD/glm/gtc/matrix_access.hpp \
    $$PWD/glm/gtc/matrix_integer.hpp \
    $$PWD/glm/gtc/matrix_inverse.hpp \
    $$PWD/glm/gtc/matrix_transform.hpp \
    $$PWD/glm/gtc/noise.hpp \
    $$PWD/glm/gtc/packing.hpp \
    $$PWD/glm/gtc/quaternion.hpp \
    $$PWD/glm/gtc/random.hpp \
    $$PWD/glm/gtc/reciprocal.hpp \
    $$PWD/glm/gtc/round.hpp \
    $$PWD/glm/gtc/type_precision.hpp \
    $$PWD/glm/gtc/type_ptr.hpp \
    $$PWD/glm/gtc/ulp.hpp \
    $$PWD/glm/gtc/vec1.hpp \
    $$PWD/glm/gtx/associated_min_max.hpp \
    $$PWD/glm/gtx/bit.hpp \
    $$PWD/glm/gtx/closest_point.hpp \
    $$PWD/glm/gtx/color_space.hpp \
    $$PWD/glm/gtx/color_space_YCoCg.hpp \
    $$PWD/glm/gtx/common.hpp \
    $$PWD/glm/gtx/compatibility.hpp \
    $$PWD/glm/gtx/component_wise.hpp \
    $$PWD/glm/gtx/dual_quaternion.hpp \
    $$PWD/glm/gtx/euler_angles.hpp \
    $$PWD/glm/gtx/extend.hpp \
    $$PWD/glm/gtx/extented_min_max.hpp \
    $$PWD/glm/gtx/fast_exponential.hpp \
    $$PWD/glm/gtx/fast_square_root.hpp \
    $$PWD/glm/gtx/fast_trigonometry.hpp \
    $$PWD/glm/gtx/gradient_paint.hpp \
    $$PWD/glm/gtx/handed_coordinate_space.hpp \
    $$PWD/glm/gtx/hash.hpp \
    $$PWD/glm/gtx/integer.hpp \
    $$PWD/glm/gtx/intersect.hpp \
    $$PWD/glm/gtx/io.hpp \
    $$PWD/glm/gtx/log_base.hpp \
    $$PWD/glm/gtx/matrix_cross_product.hpp \
    $$PWD/glm/gtx/matrix_decompose.hpp \
    $$PWD/glm/gtx/matrix_interpolation.hpp \
    $$PWD/glm/gtx/matrix_major_storage.hpp \
    $$PWD/glm/gtx/matrix_operation.hpp \
    $$PWD/glm/gtx/matrix_query.hpp \
    $$PWD/glm/gtx/matrix_transform_2d.hpp \
    $$PWD/glm/gtx/mixed_product.hpp \
    $$PWD/glm/gtx/norm.hpp \
    $$PWD/glm/gtx/normal.hpp \
    $$PWD/glm/gtx/normalize_dot.hpp \
    $$PWD/glm/gtx/number_precision.hpp \
    $$PWD/glm/gtx/optimum_pow.hpp \
    $$PWD/glm/gtx/orthonormalize.hpp \
    $$PWD/glm/gtx/perpendicular.hpp \
    $$PWD/glm/gtx/polar_coordinates.hpp \
    $$PWD/glm/gtx/projection.hpp \
    $$PWD/glm/gtx/quaternion.hpp \
    $$PWD/glm/gtx/range.hpp \
    $$PWD/glm/gtx/raw_data.hpp \
    $$PWD/glm/gtx/rotate_normalized_axis.hpp \
    $$PWD/glm/gtx/rotate_vector.hpp \
    $$PWD/glm/gtx/scalar_multiplication.hpp \
    $$PWD/glm/gtx/scalar_relational.hpp \
    $$PWD/glm/gtx/simd_mat4.hpp \
    $$PWD/glm/gtx/simd_quat.hpp \
    $$PWD/glm/gtx/simd_vec4.hpp \
    $$PWD/glm/gtx/spline.hpp \
    $$PWD/glm/gtx/std_based_type.hpp \
    $$PWD/glm/gtx/string_cast.hpp \
    $$PWD/glm/gtx/transform.hpp \
    $$PWD/glm/gtx/transform2.hpp \
    $$PWD/glm/gtx/type_aligned.hpp \
    $$PWD/glm/gtx/vector_angle.hpp \
    $$PWD/glm/gtx/vector_query.hpp \
    $$PWD/glm/gtx/wrap.hpp \
    $$PWD/glm/simd/platform.h \
    $$PWD/glm/common.hpp \
    $$PWD/glm/exponential.hpp \
    $$PWD/glm/ext.hpp \
    $$PWD/glm/fwd.hpp \
    $$PWD/glm/geometric.hpp \
    $$PWD/glm/glm.hpp \
    $$PWD/glm/integer.hpp \
    $$PWD/glm/mat2x2.hpp \
    $$PWD/glm/mat2x3.hpp \
    $$PWD/glm/mat2x4.hpp \
    $$PWD/glm/mat3x2.hpp \
    $$PWD/glm/mat3x3.hpp \
    $$PWD/glm/mat3x4.hpp \
    $$PWD/glm/mat4x2.hpp \
    $$PWD/glm/mat4x3.hpp \
    $$PWD/glm/mat4x4.hpp \
    $$PWD/glm/matrix.hpp \
    $$PWD/glm/packing.hpp \
    $$PWD/glm/trigonometric.hpp \
    $$PWD/glm/vec2.hpp \
    $$PWD/glm/vec3.hpp \
    $$PWD/glm/vec4.hpp \
    $$PWD/glm/vector_relational.hpp \
    $$PWD/png/viewfactorylibpng.h \
    $$PWD/sdl/viewfactorysdl.h \
    $$PWD/ui/button.h \
    $$PWD/core/camera.h \
    $$PWD/core/transform.h \
    $$PWD/core/view.h \
    $$PWD/core/presenter.h \
    $$PWD/core/tools.h \
    $$PWD/core/viewfactory.h \
    $$PWD/core/viewmanager.h \
    $$PWD/gl/glviewmanager.h \
    $$PWD/core/circleshape.h \
    $$PWD/core/rectshape.h \
    $$PWD/core/sprite.h \
    $$PWD/core/texture.h \
    $$PWD/gl/glviewcircle.h \
    $$PWD/gl/glviewrect.h \
    $$PWD/core/inputmgr.h \
    $$PWD/core/scenemgr.h \
    $$PWD/core/screenmgr.h \
    $$PWD/core/entity.h \
    $$PWD/core/component.h \
    $$PWD/core/entitymgr.h


SOURCES += \
    $$PWD/gl/gltools.cpp \
    $$PWD/gl/glshaderprogram.cpp \
    $$PWD/gl/glattribarray.cpp \
    $$PWD/gl/gltexture.cpp \
    $$PWD/gl/glviewfactory.cpp \
    $$PWD/gl/glworldview.cpp \
    $$PWD/gl/glviewshape.cpp \
    $$PWD/gl/glviewsprite.cpp \
    $$PWD/glut/glutmgr.cpp \
    $$PWD/qt/viewfactoryqt.cpp \
    $$PWD/core/flappyapp.cpp \
    $$PWD/core/animation.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2ChainShape.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2BroadPhase.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2CollideCircle.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2CollideEdge.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2CollidePolygon.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2Collision.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2Distance.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2DynamicTree.cpp \
    $$PWD/box2d/Box2D/Box2D/Collision/b2TimeOfImpact.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2BlockAllocator.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2Draw.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2Math.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2Settings.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2StackAllocator.cpp \
    $$PWD/box2d/Box2D/Box2D/Common/b2Timer.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Body.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2ContactManager.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Fixture.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2Island.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2World.cpp \
    $$PWD/box2d/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp \
    $$PWD/box2d/Box2D/Box2D/Rope/b2Rope.cpp \
    $$PWD/ui/button.cpp \
    $$PWD/core/camera.cpp \
    $$PWD/core/view.cpp \
    $$PWD/core/tools.cpp \
    $$PWD/core/presenter.cpp \
    $$PWD/core/viewmanager.cpp \
    $$PWD/core/circleshape.cpp \
    $$PWD/core/rectshape.cpp \
    $$PWD/core/sprite.cpp \
    $$PWD/gl/glviewcircle.cpp \
    $$PWD/gl/glviewrect.cpp \
    $$PWD/core/inputmgr.cpp \
    $$PWD/core/scenemgr.cpp \
    $$PWD/core/screenmgr.cpp \
    $$PWD/core/entitymgr.cpp

LIBS += -lGL -lglut -lGLEW -lGLU
