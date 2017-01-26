#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

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

void drawLineNegative(Point p1, Point p2, int clear){
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
	
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

void drawLinePositive(Point p1, Point p2, int clear){
	int dx = p2.x - p1.x;
	int dy = p1.y - p2.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
	
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

void drawHorizontalLine(Point p1, Point p2, int clear){
    //plotPixelRGBA(x1,y1,0,0,0,0);
    
    for(int i = p1.x ; i <= p2.x; i++){
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

void drawVerticalLine(Point p1, Point p2, int clear){
	for (int i=p1.y; i<=p2.y; i++){
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
        drawVerticalLine(p1, p2, clear);
    }
    else if (p1.y == p2.y){
        drawHorizontalLine(p1, p2, clear);
    }
    else if ((p2.x > p1.x) && (p2.y > p1.y)) {
        drawLineNegative(p1, p2, clear);
    }
    else if ((p2.x > p1.x) && (p2.y < p1.y)) {
        drawLinePositive(p1, p2, clear);
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

int main(){
	fbfd = open("/dev/fb0", O_RDWR);
     if (fbfd == -1) {
         perror("Error: cannot open framebuffer device");
         exit(1);
     }
     printf("The framebuffer device was opened successfully.\n");

     // Ambil data informasi screen
     if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
         perror("Error reading fixed information");
         exit(2);
     }
     if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
         perror("Error reading variable information");
         exit(3);
     }

     // Informasi resolusi, dan bit per pixel
     printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

     // Hitung ukuran memori untuk menyimpan framebuffer
     screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

     // Masukkan framebuffer ke memory, untuk kita ubah-ubah
     fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                        fbfd, 0);
     if ((int)fbp == -1) {
         perror("Error: failed to map framebuffer device to memory");
         exit(4);
     }
     printf("The framebuffer device was mapped to memory successfully.\n");
     
     int x;
    int y;
    for (y = 0; y < vinfo.yres - 15 ;y++){
         for (x = 0; x < vinfo.xres  ; x++) {
             if (vinfo.bits_per_pixel == 32) {
                 plotPixelRGBA(x,y,255,255,255,0);
             } else  { 
                 plotPixelRGBA(x,y,255,255,255,0);
             }

         }
	 }
     
	Point P1, P2;
	P1.x = 0; P1.y= 0; P2.x = 50; P2.y = 400;
	drawLine(P1,P2,0);

    Point P3;
    P3.x = 500; P3.y = 500; 
    int radius = 35;
    drawCircle(P3, radius, 0);
	
}
