//-------- common window parameters
typedef struct {
    int xo;
    int yo;
    int width;
    int height;
    float aspect;
	char *title;
} WindowDataT;

//-------- common mouse data
typedef struct {
	float xo, yo; // click position
	float xp, yp; // previous position
    float x,  y;  // current position
	float dx, dy; // movement
	float xAbs, yAbs; // absolute position
	float xRel, yRel; // relative position
	bool left;    // left button
	bool middle;  // middle button
 	bool right;   // right button - false:UP, true:DOWN
} MouseDataT;

//-------- common keyboard data
typedef struct {
	float x, y;
	bool charKey[128];//alpha-numeric printable ascii characters
	bool arrowLeft, arrowUp, arrowRight, arrowDown;
    bool funcKey[12]; //0:F1 - 11:F12
	bool pageUp, pageDown;
	bool home, end;
	bool insert;
} KeyDataT;
