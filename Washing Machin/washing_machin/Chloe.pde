

class Chloe
{

    float mousex = 0;


    void draw() {
        
        stroke(255, 0, 0);
        noFill();

        pushMatrix();
        translate(width/2, height/2);
        ellipse(0, 0, 100, 100);
        ellipse(0, 0, 100, 90);
        ellipse(0, 0, 100, 80);
        ellipse(0, 0, 100, 70);
        ellipse(0, 0, 100, 60);
        ellipse(0, 0, 100, 50);
        ellipse(0, 0, 100, 40);
        ellipse(0, 0, 100, 30);
        ellipse(0, 0, 100, 20);
        ellipse(0, 0, 100, 10);
        ellipse(0, 0, 100, 5);

        popMatrix();

        int a = int(map(mousex, 0, width, 0, 100));

        for (int i = 0; i < a; i++) {

            // rotate(radians(360.0/a));

            float angle;
            if (a*i==0)
                angle  =  radians(360.0);
            else
                angle  =  radians(360.0/a*i); 

            float x1 = width/2  + 50 * cos(angle); 
            float y1 = height/2 + 0 * sin(angle);

            float x2 = width/2  + 100 * cos(angle); 
            float y2 = height/2 + 100 * sin(angle);

            line(x1, y1, x2, y2);
        }
    }

    void setMouseX( float val ) {
        mousex = val;
    }
}
