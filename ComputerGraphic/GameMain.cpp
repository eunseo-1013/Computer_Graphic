


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "main.h"
#include "Camera.h"

int pov = 4;
// 0 = 사람 1 = 잠자리 2 = 사자 3 = 시각장애인1(중앙만 보임), 4 =시각장애인2(흐림), 5=시각장애인3(중앙만 안보임)

extern camera cam;

// 찐 메인 ( 게임 돌아가는 화면 )
int main(int argc, char** argv) {
	if (pov == 0) {
		cam.fov = 60.0;
	}
	else if (pov == 1) {
		cam.fov = 160.0;
	}
	else if (pov == 2) {
		cam.fov = 30;
	}
	else {
		cam.fov = 60.0; // 시각장애인
	}
	if (page == 0) {
		TomatoMain(argc, argv);

	}
	else if (page == 1) {
		CupNoodleMain(argc, argv);
	}
	return 0;
}