#include "shooter.c"

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
                 plotPixelRGBA(x,y,0,0,0,0);
             } else  { 
                 plotPixelRGBA(x,y,0,0,0,0);
             }

         }
	 }
     
	/*Point P1, P2;
	P1.x = 0; P1.y= 0; P2.x = 50; P2.y = 400;
	drawLine(P1,P2,1);


    Point P3;
    P3.x = 500; P3.y = 500; 
    int radius = 35;
    drawCircle(P3, radius, 1);
    drawCircle(P3, radius+15, 1);
    point.x = vinfo.xres/2; point.y = 0;
    drawShooter();*/

    int posx = 10;
    int posy = 10;
    Point arrPoint[38] = {
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
        
        {15,12},{9,23},
        {9,23},{5,23},
        {5,23},{8,12},
        {8,12},{9,11},
        {9,11},{0,11},
        {0,7},{0,11}
    };
    
    //scale
    int scaleFactor = 10;
    for(int i = 0; i<38; i++){
        arrPoint[i].x *= scaleFactor;
        arrPoint[i].y *= scaleFactor;
    }
    
    //position
    for (int i = 0; i<38; i++){
        arrPoint[i].x += posx;
        arrPoint[i].y += posy;
    }
    
    Point p1, p2;
    
    p1.x = 10; p1.y = 10;
    
    point.x = vinfo.xres/2;
    
    pthread_t p;
    pthread_create(&p, NULL, &commandcall, NULL);
    pthread_create(&p, NULL, &draw, NULL);
    while(1) {
        drawPlane(p1,0);
        p1.x += 1;
        drawPlane(p1,1);
        usleep(10000);
    }
    pthread_join(p, NULL);
    
    Point center = {400, 400};
    drawExplosion(center, 100);
    
}
