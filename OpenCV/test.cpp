#include "opencv2/opencv.hpp"
#include <iostream>
#include <GL/glui.h>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
//#include <GL/glut.h>

using namespace cv;
using namespace std;
int main_window,raw,rg1 = 0,point[4][2]={0};
String Filename;
Mat src1,src2,dst;
GLUI_Rollout* rollout1,* rollout2;

void open(int id){
	OpenFileDialog* openFileDialog = new OpenFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		switch (id) {
		case 0:
			src1 = imread(Filename);
			imshow("Source 1", src1);
			break;
		case 1:
			src2 = imread(Filename);
			imshow("Source 2", src2);
			break;
		}
	}
}

void save(int id){
	SaveFileDialog* openFileDialog = new SaveFileDialog();
	if (openFileDialog->ShowDialog()) {
		Filename = openFileDialog->FileName;
		imwrite(Filename, dst);
	}
}

void radiofunc(int id) {
	switch (rg1) {
	case 0:
		rollout1->enable();
		rollout2->disable();
		break;
	case 1:
		rollout1->disable();
		rollout2->enable();
		break;
	}
}

void onMouseWarp(int event, int x, int y, int flags, void* param) {
	if (event == EVENT_LBUTTONDOWN) {  // 좌측 마우스버튼을 눌렀을 때
		point[raw][0] = x;
		point[raw][1] = y;
		raw++;
		if (raw == 4) {
			Point2f inputp[4], outputp[4];
			for (int i = 0; i < 4; i++) {
				inputp[i] = Point2f(point[i][0], point[i][1]);
			}
			outputp[0] = Point2f(0, 0);			
			outputp[1] = Point2f(0, src1.rows);
			outputp[2] = Point2f(src1.cols, 0);	
			outputp[3] = Point2f(src1.cols, src1.rows);

			Mat h = getPerspectiveTransform(inputp, outputp);
			warpPerspective(src1, dst, h, src1.size());
			imshow("Source 2", dst);
			raw = 0;
		}
	}
}

void onMouseTri(int event, int x, int y, int flags, void* param) {
	if (event == EVENT_LBUTTONDOWN) {  // 좌측 마우스버튼을 눌렀을 때
		point[raw][0] = x;
		point[raw][1] = y;
		raw++;
		if (raw == 3) {
			Mat img;
			Mat Frame = src2.clone();
			namedWindow("Result");
			if (point[1][0] <= point[2][0])point[1][0] = point[2][0];
			if (point[1][1] <= point[2][1])point[1][1] = point[2][1];
			cout << "결과:" << point[0][0] << "," << point[0][1] << endl;
			cout << "결과:" << point[1][0] << "," << point[1][1] << endl;
			resize(src1, img, Size(point[1][0] - point[0][0], point[1][1] - point[0][1]));
			Mat dst(Frame, Rect(point[0][0], point[0][1], point[1][0] - point[0][0], point[1][1] - point[0][1]));
			img.copyTo(dst);
			imshow("Result", Frame);
			raw = 0;
		}
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLUI* glui = GLUI_Master.create_glui("Menu", 0);
	main_window = glui->get_glut_window_id();
	GLUI_Master.set_glutIdleFunc(NULL);

	GLUI_RadioGroup* rg = glui->add_radiogroup(&rg1, 0, radiofunc);
	glui->add_radiobutton_to_group(rg, "Project1");
	glui->add_radiobutton_to_group(rg, "Project2");

	rollout1 = glui->add_rollout("Project1", FALSE);
	GLUI_Button* btn11 = glui->add_button_to_panel(rollout1, "Source1", 0, open);
	GLUI_Button* btn12 = glui->add_button_to_panel(rollout1, "Source2", 1, open);
	GLUI_Button* btn13 = glui->add_button_to_panel(rollout1, "Result", 0, open);
	rollout1->disable();

	rollout2 = glui->add_rollout("Project2", FALSE);
	GLUI_Button* btn21 = glui->add_button_to_panel(rollout2, "Source", 0, open);
	GLUI_Button* btn22 = glui->add_button_to_panel(rollout2, "Result", 0, save);
	rollout2->disable();
	
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);
	
	namedWindow("Source 2");
	setMouseCallback("Source 2", onMouseTri);
	namedWindow("Source 1");
	setMouseCallback("Source 1", onMouseWarp);

	glui->set_main_gfx_window(main_window);
	glutMainLoop();
	return EXIT_SUCCESS;
}