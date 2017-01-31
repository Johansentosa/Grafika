#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>


typedef struct {
    int x; //absis
    int y; //ordinate
}Point;

int fbfd = 0;                       //Filebuffer Filedescriptor
struct fb_var_screeninfo vinfo;     //Struct Variable Screeninfo
struct fb_fix_screeninfo finfo;     //Struct Fixed Screeninfo
long int screensize = 0;            //Ukuran data framebuffer
char *fbp = 0;                      //Framebuffer di memori internal

int slope(Point p1, Point p2){
    int s;
    s = p2.y-p1.y / p2.x -p1.x;
    return s;
}

void plotPixelRGBA(int _x, int _y, int r, int g, int b, int a) {

		long int location = (_x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (_y+vinfo.yoffset) * finfo.line_length;
		*(fbp + location) = b;        //blue
		*(fbp + location + 1) = g;    //green
		*(fbp + location + 2) = r;    //red
		*(fbp + location + 3) = a;      //
}

void drawLineNegative(Point p1, Point p2, int clear, int dot){
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
    int print = 1;
	
    if (abs(dx) > abs(dy)) {
    	while (x != p2.x) {
            if (p < 0) {
                p = p + dy + dy;
                x++;
            }
            else {
                p = p + dy + dy - dx - dx;
                x++;
                y++;
            }
            
            if (dot!=0 && x%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    } else {
        p = 2*dx -dy;
        while (y != p2.y) {
            if (p < 0) {
                p = p + dx + dx;
                y++;
            }
            else {
                p = p + dx + dx - dy - dy;
                y++;
                x++;
            }
            
            if (dot!=0 && y%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    }

}

void drawLinePositive(Point p1, Point p2, int clear, int dot){
	int dx = p2.x - p1.x;
	int dy = p1.y - p2.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
    int print = 1;
	
    if (abs(dx) > abs(dy)) {
    	while (x != p2.x) {
            if (p < 0) {
                p = p + dy + dy;
                x++;
            }
            else {
                p = p + dy + dy - dx - dx;
                x++;
                y--;
            }
            
            if (dot!=0 && x%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    } else {
        p = 2*dx - dy;
        y = p1.y;
        while (y != p2.y) {
            if (p < 0) {
                p = p + dx + dx;
                y--;
            }
            else {
                p = p + dx + dx - dy - dy;
                y--;
                x++;
            }
            
            if (dot!=0 && y%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    }

}

void drawHorizontalLine(Point p1, Point p2, int clear, int dot){
    //plotPixelRGBA(x1,y1,0,0,0,0);
    int print = 1;
    for(int i = p1.x ; i <= p2.x; i++){
        if (dot!=0 && i%dot == 0) print = !print;
        if (!print) continue;
        if (clear == 1) {
          plotPixelRGBA(i,p1.y,255,255,255,0);
        }
        else if (clear == 2) {
          plotPixelRGBA(i,p1.y,255,0,0,0);
        }
        else {
          plotPixelRGBA(i,p1.y,0,0,0,0);
        }
    }
}

void drawVerticalLine(Point p1, Point p2, int clear, int dot){
    int print = 1;
	for (int i=p1.y; i<=p2.y; i++){
        if (dot!=0 && i%dot == 0) print = !print;
        if (!print) continue;
		if (clear ==1){
			plotPixelRGBA(p1.x,i,255,255,255,0);
        }
        else if (clear == 2) {
			plotPixelRGBA(p1.x,i,255,0,0,0);
        }
        else {
			plotPixelRGBA(p1.x,i,0,0,0,0);
        }
	}	
}

void drawLine(Point p1, Point p2, int clear){
	if (p1.x > p2.x) {
        drawLine(p2, p1, clear);
    }
    else if (p1.x == p2.x) {
        drawVerticalLine(p1, p2, clear,0);
    }
    else if (p1.y == p2.y){
        drawHorizontalLine(p1, p2, clear,0);
    }
    else if ((p2.x > p1.x) && (p2.y > p1.y)) {
        drawLineNegative(p1, p2, clear, 0);
    }
    else if ((p2.x > p1.x) && (p2.y < p1.y)) {
        drawLinePositive(p1, p2, clear,0);
    }
}

void drawDottedLine(Point p1, Point p2, int clear, int dot){
    if (p1.x > p2.x) {
        drawDottedLine(p2, p1, clear, dot);
    }
    else if (p1.x == p2.x) {
        drawVerticalLine(p1, p2, clear, dot);
    }
    else if (p1.y == p2.y){
        drawHorizontalLine(p1, p2, clear, dot);
    }
    else if ((p2.x > p1.x) && (p2.y > p1.y)) {
        drawLineNegative(p1, p2, clear, dot);
    }
    else if ((p2.x > p1.x) && (p2.y < p1.y)) {
        drawLinePositive(p1, p2, clear, dot);
    }
}

// draw circle section

void drawCircle(Point center, int radius, int clear) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;
    /* plot first set of point */
    plot8points(center, x, y, clear);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        plot8points(center, x, y, clear);
    }
}

void plot4points(Point center, int x, int y, int clear) {
    if (clear == 1) {
        plotPixelRGBA(center.x + x, center.y + y, 255, 255, 255, 0);
        plotPixelRGBA(center.x - x, center.y + y, 255, 255, 255, 0);
        plotPixelRGBA(center.x + x, center.y - y, 255, 255, 255, 0);
        plotPixelRGBA(center.x - x, center.y - y, 255, 255, 255, 0);
    } else 
    if (clear == 2) {
        plotPixelRGBA(center.x + x, center.y + y, 255, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y + y, 255, 0, 0, 0);
        plotPixelRGBA(center.x + x, center.y - y, 255, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y - y, 255, 0, 0, 0);
    } else {
        plotPixelRGBA(center.x + x, center.y + y, 0, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y + y, 0, 0, 0, 0);
        plotPixelRGBA(center.x + x, center.y - y, 0, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y - y, 0, 0, 0, 0);
    }
        
}

void plot8points(Point center, int x, int y, int clear) {
        plot4points(center, x, y, clear);
        plot4points(center, y, x, clear);
}

void drawPlane (Point start){
	int posx = start.x;
    int posy = start.y;
    
    int nPoint = 58;
    Point arrPoint[58] = {
		//tail 10
		{0,2}, {2,2},
		{2,2}, {5,6},
		{5,6}, {9,8},
		{9,8}, {1,8},
		{1,8}, {0,2},
		
		//body 28
		{0,7},{1,7},
		{7,7},{19,7},
		{23,7},{25,7},
		{25,7},{30,9},
		{30,9},{25,11},
		{25,11},{18,11},
		{18,11},{17,12},
		{17,12},{15,12},
		{15,12},{9,21},
		{9,21},{5,21},
		{5,21},{8,12},
		{8,12},{9,11},
		{9,11},{0,11},
		{0,7},{0,11},
		
		//kaca 12
		{19,7},{20,6},
		{19,7},{20,8},
		{22,6},{23,7},
		{22,8},{23,7},
		{20,6},{22,6},
		{20,8},{22,8},
		
		//sayap 6
		{10,7},{17,0},
		{17,0},{19.5,0},
		{19.5,0},{18,7},
		
		//roda 2
		{19,11},{19,12}
	};
	
	Point pCircle = {19,13};
	
	//scale
    int scaleFactor = 10;
    for(int i = 0; i<nPoint; i++){
		arrPoint[i].x *= scaleFactor;
		arrPoint[i].y *= scaleFactor;
	}
	pCircle.x *= scaleFactor;
	pCircle.y *= scaleFactor;
	
	//position
	for (int i = 0; i<nPoint; i++){
		arrPoint[i].x += posx;
		arrPoint[i].y += posy;
	}
	pCircle.x += posx;
	pCircle.y += posy;
	
    //draw
    for (int i = 0; i<nPoint; i = i+2){
		drawLine(arrPoint[i],arrPoint[i+1],1);
	}
	
	drawCircle(pCircle,8,1);
	drawCircle(pCircle,4,1);
}

void drawPlaneRight (Point start){
	int posx = start.x;
	int posy = start.y;
	
	//copas void drawPlane sorry soalnya gamau jadiin arrPoint nya parameter
	int nPoint = 58;
    Point arrPoint[58] = {
		//tail 10
		{0,2}, {2,2},
		{2,2}, {5,6},
		{5,6}, {9,8},
		{9,8}, {1,8},
		{1,8}, {0,2},
		
		//body 28
		{0,7},{1,7},
		{7,7},{19,7},
		{23,7},{25,7},
		{25,7},{30,9},
		{30,9},{25,11},
		{25,11},{18,11},
		{18,11},{17,12},
		{17,12},{15,12},
		{15,12},{9,21},
		{9,21},{5,21},
		{5,21},{8,12},
		{8,12},{9,11},
		{9,11},{0,11},
		{0,7},{0,11},
		
		//kaca 12
		{19,7},{20,6},
		{19,7},{20,8},
		{22,6},{23,7},
		{22,8},{23,7},
		{20,6},{22,6},
		{20,8},{22,8},
		
		//sayap 6
		{10,7},{17,0},
		{17,0},{19.5,0},
		{19.5,0},{18,7},
		
		//roda 2
		{19,11},{19,12}
	};
	
	Point pCircle = {-19,13};
	
	//reflect
	for(int i = 0; i<nPoint; i++){
		arrPoint[i].x *=-1;
	}
	
	//scale
    int scaleFactor = 10;
    for(int i = 0; i<nPoint; i++){
		arrPoint[i].x *= scaleFactor;
		arrPoint[i].y *= scaleFactor;
	}
	pCircle.x *= scaleFactor;
	pCircle.y *= scaleFactor;
	
	//position
	for (int i = 0; i<nPoint; i++){
		arrPoint[i].x += posx;
		arrPoint[i].y += posy;
	}
	pCircle.x += posx;
	pCircle.y += posy;
	
    //draw
    for (int i = 0; i<nPoint; i = i+2){
		drawLine(arrPoint[i],arrPoint[i+1],1);
	}
	
	drawCircle(pCircle,8,1);
	drawCircle(pCircle,4,1);
}

void drawExplosion(Point center, int scale) {
	Point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
	p1.x = center.x - (1*scale);		p1.y = center.y + (2*scale);
	p2.x = center.x;					p2.y = center.y + (3*scale);
	p3.x = center.x + (1*scale);		p3.y = center.y + (2*scale);
	p4.x = center.x + (3*scale);		p4.y = center.y + (3*scale);
	p5.x = center.x + (2*scale);		p5.y = center.y + (1*scale);
	p6.x = center.x + (3*scale);		p6.y = center.y;
	p7.x = center.x + (2*scale);		p7.y = center.y - (1*scale);
	p8.x = center.x + (3*scale);		p8.y = center.y - (3*scale);
	p9.x = center.x + (1*scale);		p9.y = center.y - (2*scale);
	p10.x = center.x;					p10.y = center.y - (3*scale);
	p11.x = center.x - (1*scale);		p11.y = center.y - (2*scale);
	p12.x = center.x - (3*scale);		p12.y = center.y - (3*scale);
	p13.x = center.x - (2*scale);		p13.y = center.y - (1*scale);
	p14.x = center.x - (3*scale);		p14.y = center.y;
	p15.x = center.x - (2*scale);		p15.y = center.y + (1*scale);
	p16.x = center.x - (3*scale);		p16.y = center.y + (3*scale);
	
	drawLine(p1,p2,0);
	drawLine(p2,p3,0);
	drawLine(p3,p4,0);
	drawLine(p4,p5,0);
	drawLine(p5,p6,0);
	drawLine(p6,p7,0);
	drawLine(p7,p8,0);
	drawLine(p8,p9,0);
	drawLine(p9,p10,0);
	drawLine(p10,p11,0);
	drawLine(p11,p12,0);
	drawLine(p12,p13,0);
	drawLine(p13,p14,0);
	drawLine(p14,p15,0);
	drawLine(p15,p16,0);
	drawLine(p16,p1,0);
}
