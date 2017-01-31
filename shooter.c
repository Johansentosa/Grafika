#include "drawer.c"
#include <termios.h>

Point point;

char getch() {
    char buf = 0;
    struct termios old = {0};
    struct termios new = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    new = old;
    new.c_lflag &= ~ICANON;
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &new) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

void drawShooter() {
	Point p1, p2;
	
	p1.x = vinfo.xres/2; p1.y = vinfo.yres * 0.95;
	drawDottedLine(point, p1, 1, 10);
}

void shoot() {
	int radius = 10;
	Point p1;
	p1.x = vinfo.xres/2; p1.y = vinfo.yres * 0.95;
	drawCircle(p1, radius, 1);

	if (point.x == vinfo.xres/2) {
		for (int i=(vinfo.yres * 0.95); i>point.y+radius; i--){
	        drawCircle(p1, radius, 0);
	        p1.y = i;
	        drawCircle(p1, radius, 1);
	        usleep(400);
		}	
		drawCircle(p1, radius, 0);
	} else if (point.x < vinfo.xres/2) {
		
	}
}

void *commandcall() {
    while (1) {
        char X = getch();
        if (X == '\033') {
        	getch();
        	X = getch();
        	if (X == 'C') {
        		if (point.x < vinfo.xres - 100)
        			point.x += 100;
        	} else if (X == 'D') {
        		if (point.x > 100)
        			point.x -= 100;
        	}
            
        } else if (X == ' ') {
        	Point p1;
        	p1.x = vinfo.xres/2; p1.y = vinfo.yres * 0.95;
        	shoot();
        }
        

        drawShooter();
    }
}