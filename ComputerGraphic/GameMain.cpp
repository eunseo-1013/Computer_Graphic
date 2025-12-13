


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "main.h"

#include "Camera.h"
int page = 1;


int animal = 1;
// 0 = 사람 1 = 잠자리 2 = 강아지

extern camera cam;

// 찐 메인 ( 게임 돌아가는 화면 )
int main(int argc, char** argv) {
	if (animal == 0) {
		cam.fov = 60.0;
	}
	else if (animal == 1) {
		cam.fov = 160.0;
	}
	else if (animal == 2) {
		cam.fov = 90;
	}
	if (page == 0) {
		TomatoMain(argc, argv);

	}
	else if (page == 1) {
		CupNoodleMain(argc, argv);
	}
	return 0;
}