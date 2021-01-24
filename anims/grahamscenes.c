#include <stdio.h>
#include <xtango.h>

void ANIMInit()
{
    ASSOCinit();
    ASSOCmake("XVALUES", 1);
    ASSOCmake("YVALUES", 1);
    ASSOCmake("IMG_ID", 1);
    ASSOCmake("IMG_LABEL", 1);
}

void ANIMDraw(x, y, id)
int x, y, id;
{
    TANGO_IMAGE circle,text;
    TANGO_TRANS delay,num;
    TANGO_PATH path;
    double px,py;
    char str[4];

    px =(double) x / 1000;
    py = (double) y / 1000;
    ASSOCstore("XVALUES", id, x);
    ASSOCstore("YVALUES", id, y); 
    circle = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
	         px, py, 1, TANGO_COLOR_BLUE, 0.02, 0.0);
    path = TANGOpath_null(10);
    sprintf(str, "%d", id);
    text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,
		 px, py, 1, TANGO_COLOR_BLACK, NULL, str, 1);
    num = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, text, path);
    ASSOCstore("IMG_ID", id, circle);
    ASSOCstore("IMG_LABEL", circle, text);
    delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, circle, path);
    delay = TANGOtrans_compose(2, delay, num);
    TANGOtrans_perform(delay);
    TANGOpath_free(1, path);
    TANGOtrans_free(2, delay, num);
}

void ANIMFindMin(id1, id2) 
int id1, id2;
{
    TANGO_IMAGE circle1, circle2;
    TANGO_PATH path, fillpath;
    TANGO_TRANS trans1, trans2, compose;
    double blink[4];

    circle1 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id1);
    circle2 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id2);
    blink[0] = 1.0; blink[2] = -1.0; blink[1] = blink[3] = 0.0;
    path = TANGOpath_create(4, blink, blink);
    fillpath = TANGOpath_iterate(path, 16.0);
    trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, circle1, fillpath);
    trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, circle2, fillpath);
    compose = TANGOtrans_compose(2, trans1, trans2);
    TANGOtrans_perform(compose);

    TANGOpath_free(2, path, fillpath);
    TANGOtrans_free(3, trans1, trans2, compose);
}

void ANIMSetMin(id)
int id;
{
    TANGO_IMAGE circle;
    TANGO_PATH path;
    TANGO_TRANS trans;
 
    circle = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id);
    path = TANGOpath_color(TANGO_COLOR_RED);
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, circle, path);
    TANGOtrans_perform(trans);

    TANGOpath_free(1, path);
    TANGOtrans_free(1, trans);
}

void ANIMUnSetMin(id)
int id;
{
    TANGO_IMAGE circle;
    TANGO_PATH path;
    TANGO_TRANS trans;
 
    circle = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id);
    path = TANGOpath_color(TANGO_COLOR_BLUE);
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, circle, path);
    TANGOtrans_perform(trans);

    TANGOpath_free(1, path);
    TANGOtrans_free(1, trans);
}

void ANIMSwap(id1, id2)
int id1, id2;
{
    TANGO_IMAGE circle1, circle2, text1, text2;
    TANGO_LOC loc1, loc2;
    TANGO_PATH expath1, expath2, path1, path2;
    TANGO_TRANS move[4], compose;
    int p1x, p1y, p2x, p2y;

    circle1 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id1);
    circle2 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id2);
    loc1 = TANGOimage_loc(circle1, TANGO_PART_TYPE_C);
    loc2 = TANGOimage_loc(circle2, TANGO_PART_TYPE_C);
    text1 = (TANGO_IMAGE) ASSOCretrieve("IMG_LABEL", circle1);
    text2 = (TANGO_IMAGE) ASSOCretrieve("IMG_LABEL", circle2);
    path1 = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_CLOCKWISE);
    path2 = TANGOpath_rotate(path1, 180);
    move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, circle1, path1);
    move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, text1, path1);
    move[2] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, circle2, path2);
    move[3] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, text2, path2);
    compose = TANGOtrans_compose(4, move[0], move[1], move[2], move[3]);
    TANGOtrans_perform(compose);

    p1x = (int) ASSOCretrieve("XVALUES", id1);
    p1y = (int) ASSOCretrieve("YVALUES", id1);
    p2x = (int) ASSOCretrieve("XVALUES", id2);
    p2y = (int) ASSOCretrieve("YVALUES", id2);

    ASSOCstore("XVALUES", id1, p2x);
    ASSOCstore("YVALUES", id1, p2y);
    ASSOCstore("XVALUES", id2, p1x);
    ASSOCstore("YVALUES", id2, p1y);

    TANGOpath_free(2, path1, path2);
    TANGOtrans_free(5, move[0], move[1], move[2], move[3], compose);
}

void ANIMSelectPt(id)
int id;
{
    TANGO_IMAGE circle;
    TANGO_PATH color, fill;
    TANGO_TRANS trans1, trans2, compose;
    double f = 1.0;
 
    circle = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id);
    color = TANGOpath_color(TANGO_COLOR_ORANGE);
    trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, circle, color);
    fill = TANGOpath_create(1, &f, &f);
    trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, circle, fill);
    compose = TANGOtrans_compose(2, trans1, trans2);
    TANGOtrans_perform(compose);

    TANGOpath_free(2, color, fill);
    TANGOtrans_free(3, trans1, trans2, compose);
}

void ANIMUnSelectPt(id)
int id;
{
    TANGO_IMAGE circle;
    TANGO_PATH color, fill;
    TANGO_TRANS trans1, trans2, compose;
    double f = 1.0;
 
    circle = (TANGO_IMAGE) ASSOCretrieve("IMG_ID", id);
    color = TANGOpath_color(TANGO_COLOR_GREEN);
    trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, circle, color);
    fill = TANGOpath_create(1, &f, &f);
    trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, circle, fill);
    compose = TANGOtrans_compose(2, trans1, trans2);
    TANGOtrans_perform(compose);

    TANGOpath_free(2, color, fill);
    TANGOtrans_free(3, trans1, trans2, compose);
}

void ANIMCompAngle(id0, id1, id2)
int id0, id1, id2;
{
    double p0x, p0y, p1x, p1y, p2x, p2y, blink[4];
    TANGO_IMAGE linep0_p1, linep1_p2, img;
    TANGO_PATH path, fillpath;
    TANGO_TRANS line1, line2, compose;
    char str[4];

    p0x = (double) ASSOCretrieve("XVALUES", id0) / 1000;
    p0y = (double) ASSOCretrieve("YVALUES", id0) / 1000;
    p1x = (double) ASSOCretrieve("XVALUES", id1) / 1000;
    p1y = (double) ASSOCretrieve("YVALUES", id1) / 1000;
    p2x = (double) ASSOCretrieve("XVALUES", id2) / 1000;
    p2y = (double) ASSOCretrieve("YVALUES", id2) / 1000;

    path = TANGOpath_null(0);
    linep0_p1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, p0x, p0y,
		    1, TANGO_COLOR_MAROON, (p1x - p0x), (p1y - p0y), 
		    0.0, 0.0, 0);
    line1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, linep0_p1, path);
    linep1_p2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, p1x, p1y,
		    1, TANGO_COLOR_MAROON, (p2x - p1x), (p2y - p1y),
		    0.0, 0.0, 0);
    line2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, linep1_p2, path);
    blink[0] = 1.0; blink[2] = -1.0; blink[1] = blink[3] = 0.0;
    path = TANGOpath_create(4, blink, blink);
    fillpath = TANGOpath_iterate(path, 16.0);
    line1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, linep0_p1, fillpath);
    line2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, linep1_p2, fillpath);  
    compose = TANGOtrans_compose(2, line1, line2);
    TANGOtrans_perform(compose);

    path = TANGOpath_null(1);
    line1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, linep0_p1, path);
    line2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, linep1_p2, path);  
    compose = TANGOtrans_compose(2, line1, line2);
    TANGOtrans_perform(compose);

    TANGOpath_free(2, path, fillpath);
    TANGOtrans_free(3, line1, line2, compose);
}

void ANIMDrawConvHull(id0, id1)
int id0, id1;
{
    double p0x, p0y, p1x, p1y;
    TANGO_IMAGE linep0_p1;
    TANGO_PATH path;
    TANGO_TRANS line;

    p0x = (double) ASSOCretrieve("XVALUES", id0) / 1000;
    p0y = (double) ASSOCretrieve("YVALUES", id0) / 1000;
    p1x = (double) ASSOCretrieve("XVALUES", id1) / 1000;
    p1y = (double) ASSOCretrieve("YVALUES", id1) / 1000;

    path = TANGOpath_null(1);
    linep0_p1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, p0x, p0y,
		    1, TANGO_COLOR_BLUE, (p1x - p0x), (p1y - p0y), 
		    0.5, 1.0, 0);
    line = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, linep0_p1, path);
    TANGOtrans_perform(line);

    TANGOpath_free(1, path);
    TANGOtrans_free(1, line);
}

