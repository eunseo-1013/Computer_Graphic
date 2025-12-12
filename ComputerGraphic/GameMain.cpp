


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "main.h"


int page = 0;

// 찐 메인 ( 게임 돌아가는 화면 )
int main(int argc, char ** argv) {
	if (page == 0) {
		TomatoMain(argc, argv);

	}
	else if (page == 1) {
		CupNoodleMain(argc, argv);
	}


	return 0;
}