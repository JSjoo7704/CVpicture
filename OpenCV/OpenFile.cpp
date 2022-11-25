#include "opencv2/opencv.hpp"
//#include <GL/glut.h>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"

using namespace cv;

int main_window;
String Filename;
Mat src;

void open(int id)
{
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	Mat src;
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		src = imread(Filename);
		imshow("src", src);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();
	GLUI_Master.set_glutIdleFunc(NULL);

	new GLUI_Button(glui, "Open", 0, open);
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}