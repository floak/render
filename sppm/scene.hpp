#ifndef __SCENE_H__
#define __SCENE_H__

#include "obj.hpp"




Object* room[] = {
	new SphereObject(vec3(1e5+1,40.8,81.6),   1e5, DIFF, 1.5, vec3(0.75,0.25,0.25)),//Left
	new SphereObject(vec3(-1e5+299,40.8,81.6), 1e5, DIFF, 1.5, vec3(0.75,0.25,0.25)),//Right
	new SphereObject(vec3(50,40.8, 1e5),      1e5, DIFF, 1.5, vec3(1,1,1)),//Back
	new SphereObject(vec3(50,40.8,-1e5+190),  1e5, DIFF, 1.5, vec3(.25,.25,.25)),//Front
	new SphereObject(vec3(50, 1e5, 81.6),     1e5, SPEC, 1.5, vec3(1,1,1)),//Botrom
	new SphereObject(vec3(50,-1e5+81.6,81.6), 1e5, DIFF, 1.5, vec3(.75,.75,.75)),//Top
	new SphereObject(vec3(27,16.5,47),       16.5, SPEC, 1.5, vec3(1,1,1)),//Mirror
	//new   CubeObject(vec3(0,8,0),    vec3(30,10,30), DIFF, 1.5, vec3(76/255.,34/255.,27/255.)),
	//new BezierObject(vec3(15, 9.99, 15),   bezier, DIFF, 1.7, vec3(1,1,1)*.999, vec3(), "vase.png"),
	new SphereObject(vec3(73,16.5,40),       16.5, REFR, 1.7, vec3(1,1,1)),//Main Ball
	//new SphereObject(vec3(45,6,45),             6, REFR, 1.7, vec3(.5,.5,1)*.999),//SmallBall0
	//new SphereObject(vec3(52,3,75),             3, REFR, 1.7, vec3(1,.5,.5)*.999),//SmallBall1
	new SphereObject(vec3(65.5,7,88),           7, REFR, 1.7, vec3(1,1,1)),//SmallBall2
	new SphereObject(vec3(77,3,92),             3, REFR, 1.7, vec3(1,1,.5)),//SmallBall3
	// new SphereObject(vec3(16,60,100),          12, REFR, 1.5, vec3(1,1,1)*.999),//FlyBall
	new SphereObject(vec3(50,681.6-.27,81.6), 600, DIFF, 1.5, vec3(), vec3(1,1,1)*20) //Lite
};

Object** scene = room;
int scene_num = 11;

// std::pair<Refl_t, vec3> get_feature(Object* obj, Texture&texture, vec3 x) {
// 	std::pair<Refl_t, vec3> feature;
	
// 	feature = texture.getcol(x.z, x.x);
// 	return feature;
// }




#endif // __SCENE_H__
