#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

#define LINE_DDA 1
#define LINE_MIDPOINT 2
#define LINE_PARAMETRIC 3
#define CIRCLE_DIRECT 4
#define CIRCLE_POLAR 5
#define CIRCLE_ITERATIVE 6
#define CIRCLE_MIDPOINT 7
#define CIRCLE_MODIFIED 8
#define ELLIPSE_DIRECT 9
#define ELLIPSE_POLAR 10
#define CLIPPING_POINT 11
#define CLIPPING_LINE 12

#define EXIT 13
#define CLEAR 14
#define RED 15
#define BLUE 16
#define GREEN 17
#define SAVE 18
#define LOAD 19


HMENU menu;
COLORREF color=RGB(256,256,256);
string colorName="black";



/*
g. Implement a save function to save all data in the screen. [2]
h. Implement a load function to load the data from files. [2]
*/

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void addMenu(HWND hwnd)
{
    menu=CreateMenu();
    HMENU dropList=CreateMenu();

    HMENU line=CreateMenu();
    HMENU circle=CreateMenu();
    HMENU ellipse=CreateMenu();
    HMENU clipping=CreateMenu();
    HMENU color=CreateMenu();

    //for line
    AppendMenu(line,MF_STRING,LINE_DDA,"DDA");
    AppendMenu(line,MF_STRING,LINE_MIDPOINT,"Midpoint");
    AppendMenu(line,MF_STRING,LINE_PARAMETRIC,"parametric");

    //circle
    AppendMenu(circle,MF_STRING,CIRCLE_DIRECT,"Direct");
    AppendMenu(circle,MF_STRING,CIRCLE_POLAR,"Polar");
    AppendMenu(circle,MF_STRING,CIRCLE_ITERATIVE,"Iterative Polar");
    AppendMenu(circle,MF_STRING,CIRCLE_MIDPOINT,"midpoint");
    AppendMenu(circle,MF_STRING,CIRCLE_MODIFIED,"modified midpoint");

    //ellipse
    AppendMenu(ellipse,MF_STRING,ELLIPSE_DIRECT,"Direct");
    AppendMenu(ellipse,MF_STRING,ELLIPSE_POLAR,"Polar");
    //clipping
    AppendMenu(clipping,MF_STRING,CLIPPING_POINT,"Point");
    AppendMenu(clipping,MF_STRING,CLIPPING_LINE,"Line");

    //color
    AppendMenu(color,MF_STRING,RED,"Red");
    AppendMenu(color,MF_STRING,BLUE,"Blue");
    AppendMenu(color,MF_STRING,GREEN,"Green");


    AppendMenu(dropList,MF_POPUP,(UINT_PTR)color,"Color");
    AppendMenu(dropList,MF_POPUP,(UINT_PTR)line,"Line");
    AppendMenu(dropList,MF_POPUP,(UINT_PTR)circle,"Circle");
    AppendMenu(dropList,MF_POPUP,(UINT_PTR)ellipse,"Ellipse");
    AppendMenu(dropList,MF_POPUP,(UINT_PTR)clipping,"Clipping");

    AppendMenu(dropList,MF_STRING,CLEAR,"Clear");
    AppendMenu(dropList,MF_STRING,SAVE,"Save");
    AppendMenu(dropList,MF_STRING,LOAD,"Load");
    AppendMenu(dropList,MF_STRING,EXIT,"exit");

    AppendMenu(menu,MF_POPUP,(UINT_PTR)dropList,"File");

    SetMenu(hwnd,menu);
}


void parametricline(HDC hdc,int x1,int y1,int x2,int y2){
	double dx=x2-x1;
	double dy=y2-y1;
	for(double t=0;t<1;t+=0.001){
    		int x=x1+(dx*t);
    		int y=y1+(dy*t);
    		SetPixel(hdc,x,y,RGB(250,1,1));
	}

}
void DDC(HDC hdc,int xs, int ys, int xe, int ye){
    int dx=xe-xs;
    int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
        int x=xs,xinc= dx>0?1:-1;
        double y=ys,yinc=(double)dy/dx*xinc;
        while(x!=xe)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,x,round(y),color);
        }
    }
    else
    {
        int y=ys,yinc= dy>0?1:-1;
        double x=xs,xinc=(double)dx/dy*yinc;
        while(y!=ye)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,round(x),y,color);
        }
    }
}
void midPoint(HDC hdc,int X1, int Y1, int X2, int Y2)
{
    int C=0;
    if(X1 > X2 && Y1 < Y2)
    {
        swap(X1,X2);
        swap(Y1,Y2);
        C = 1;
    }
    if(X1 < X2 && Y1 > Y2)
    {
        C=1;
    }
    if(X1 > X2 && Y1 > Y2)
    {
        swap(X1,X2);
        swap(Y1,Y2);
    }
    int dx = X2 - X1;
    int dy = Y2 - Y1;
    if(dy<=dx){
        int d = dx - 2*dy;
        double x = X1, y = Y1;
        double c1=(dx-dy);
        int c2= -dy;
        SetPixel(hdc,x,y,color);

        while (x < X2)
        {
            x++;
            if (d <= 0)
            {
                d = d + c1;
                y++;
            }
            else
            {
                if(C == 1)
                {
                    y=(((Y2-Y1)*(x-X1))/(X2-X1))+Y1;
                }
                d += c2;

            }
            SetPixel(hdc,x,y,color);
        }
    }

}


void draweight(HDC hdc,int x,int y,int xc,int yc,int i){
    //1
    if(i == 1){
        DDC(hdc,xc,yc,xc+x,yc-y);
    //2
    }
    else if(i == 2)
        {DDC(hdc,xc,yc,xc+y,yc+x);
    //4
        DDC(hdc,xc,yc,xc+x,yc+y);}
    //5
    else if(i == 3)
        {DDC(hdc,xc,yc,xc-x,yc+y);
    //6
        DDC(hdc,xc,yc,xc-y,yc+x);}
    //7
    else if(i == 4)
        {DDC(hdc,xc,yc,xc-y,yc-x);
    //8
        DDC(hdc,xc,yc,xc-x,yc-y);}
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
    SetPixel(hdc,xc-x,yc-y,color);
}


void CircleDirect(HDC hdc,int xc,int yc, int R,int c){
    int x=0,y=R;
    int R2=R*R;
    draweight(hdc,x,y,xc,yc,c);
    while(x < y)
    {
        x++;
        y=sqrt((double)(R2-x*x));
        draweight(hdc,x,y,xc,yc,c);
    }
}
void CirclePolar(HDC hdc,int xc,int yc, int R,int c){

    int x=R,y=0;
    double theta=0,dtheta=1.0/R;
    draweight(hdc,x,y,xc,yc,c);
    while(x>y)
    {
        theta+=dtheta;
        x=round(R*cos(theta));
        y=round(R*sin(theta));
        draweight(hdc,x,y,xc,yc,c);
    }
}
void CircleIterativePolar(HDC hdc,int xc,int yc, int R,int c){

    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    draweight(hdc,x,y,xc,yc,c);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        draweight(hdc,round(x),round(y),xc,yc,c);
    }
}
void midpoint(HDC hdc,int xc,int yc,int r,int c){
    int x=0;
    int y=r;
    draweight(hdc,x,y,xc,yc,c);
    double d=1-r;
    while(x<y){
        if(d<=0){
            d+=2*x+2;
            x++;
        }
        else{
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        draweight(hdc,x,y,xc,yc,c);
    }
}
void modfmidpoint(HDC hdc,int xc,int yc,int r,int c){
    int x=0;
    int y=r;
    double d=1-r;
    int c1 = 3;
    int c2 = 5 - 2*r;
    draweight(hdc,x,y,xc,yc,c);
    while(x<y){
        if(d<0){
            d=d+c1;
            c2+=2;
            x++;
        }
        else{
            d+=c2;
            c2+=4;
            x++;
            y--;
        }
        c1+=2;
        draweight(hdc,x,y,xc,yc,c);
    }
}


void PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom){
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);
    }
    else{
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);
        SetPixel(hdc,x,y,color);

    }
}

union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {

        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}
void drawFour(HDC hdc, int Cx, int Cy, int x, int y)
{
    SetPixel(hdc, Cx+x, Cy+y, color);
    SetPixel(hdc, Cx-x, Cy+y, color);
    SetPixel(hdc, Cx-x, Cy-y, color);
    SetPixel(hdc, Cx+x, Cy-y, color);
}

void polarEllipse(HDC hdc, int cx, int cy, int rx, int ry, double R_ratio)
{
    double thetaStart=0.0, thetaEnd=90.0;
    double Rx=sqrt(pow((cx-rx),2)+pow((cy-ry),2));
    double Ry=Rx/R_ratio;
    while(thetaStart<thetaEnd)
    {
        int x = Rx*cos(thetaStart);
        int y = Ry*sin(thetaStart);
        drawFour(hdc, cx, cy, x, y);
        thetaStart+=0.01;
    }
}


void directEllipse(HDC hdc,int cx,int cy, int Rx, double R_ratio)
{
    /* [(X-Xc)^2]/A^2 + [(Y-Yc)^2]/B^2 = 1 */
    float B= Rx/R_ratio;                           //C=sqrt(R*R-B*B), E=C/R;
    int y=B, x=0;                                   //, xFociPov=cx+C;//, xFociNeg=cx-c;                        //y=R, y2=R/2, Ry2=((R*R)/4);

    while(x<=y)
    {
        y=sqrt(((B*B)- ((B*B*x*x)/(Rx*Rx))));

        drawFour(hdc, cx, cy, x, y);
        x++;

    }
    while(y<=x)
    {
        x=sqrt(((Rx*Rx)- ((Rx*Rx*y*y)/(B*B))));

        drawFour(hdc, cx, cy, x, y);
        y--;

    }
    /*while(x<y/2)
    {
        x++;
        y=round(sqrt((double)((R*R)-x*x)))-(R/1.9);

        SetPixel(hdc, cx+x, cy+y, RGB(256,256,256));
        SetPixel(hdc, cx-x, cy+y, RGB(256,256,256));
        SetPixel(hdc, cx-x, cy-y, RGB(256,256,256));
        SetPixel(hdc, cx+x, cy-y, RGB(256,256,256));
    }

    while(x<y)
    {
        y2=round(sqrt((double)(Ry2-x*x)));
        y=round(sqrt((double)((R*R)-x*x)));
        x++;
        SetPixel(hdc, cx+x, (cy+y2), RGB(256,256,256));
        SetPixel(hdc, cx-x, (cy+y2), RGB(256,256,256));
        SetPixel(hdc, cx+x, (cy-y2), RGB(256,256,256));
        SetPixel(hdc, cx-x, (cy-y2), RGB(256,256,256));
    }*/
}
vector<string>shape;
vector<string>colors;
vector<int>points;
void pushVectors(int x_c,int y_c,int x_2,int y_2,int x3,int y3,int x4,int y4,int rr,int c,string shapeName,int num)
{
    points.push_back(x_c);
    points.push_back(y_c);
    points.push_back(x_2);
    points.push_back(y_2);
    points.push_back(x3);
    points.push_back(y3);
    points.push_back(x4);
    points.push_back(y4);
    points.push_back(rr);
    points.push_back(c);
    points.push_back(num);
    shape.push_back(shapeName);

    colors.push_back(colorName);
}


void save_toFile()
{
    ofstream saved("saved.txt");

    for(int i=0;i<shape.size();i++)
    {
        saved<<shape[i]<<" "<<colors[i]<<endl;
        int points_start=i*11, points_end=points_start+11;
        for(int j=points_start; j<points_end; j++)
            saved<<points[j]<<" ";
        saved<<endl;
    }
    saved.close();
}



void load_fromFile()
{
    string getShape,getColor, output;
    int getPoints[11];
    ifstream saved;//("saved.txt");
    saved.open("saved.txt");

    int i=0,j=0;
    while(saved>>output)
    {
        if(i==0)
            getShape=output;
        else if(i==1)
            getColor=output;
        else{
            if(j<10)
            {
                getPoints[j]=stoi(output);
                j++;
            }
            else{
                getPoints[j]=stoi(output);
                j=0;
                i=-1;
                shape.push_back(getShape);  colors.push_back(getColor);
                //cout<<getShape<<" "<<getColor<<endl;
                for(int n=0;n<11;n++){
                    points.push_back(getPoints[n]);
                    //cout<<getPoints[n]<<" ";
                }
                //cout<<endl;
            }
        }
        i++;
    }
    saved.close();
}


void drawAll(HDC hdc)
{
    cout<<"draw all\n";
    int num=0;
    int indexpoints=0;
    int rr=0,x_c=0,y_c=0,x_2=0,y_2=0,c = 0,x3=0,y3=0,x4=0,y4=0;
    for(int i=0;i<shape.size();i++)
    {
        cout<<i<<endl;
        if(colors[i]=="blue")
        {
            color=RGB(0,0,255);
            colorName="blue";
        }
        else if(colors[i]=="green")
        {
            color=RGB(0,255,0);
            colorName="green";
        }
        else if(colors[i]=="red")
        {
            color=RGB(255,0,0);
            colorName="red";
        }
        else
        {
            color=RGB(256,256,256);
            colorName="black";
        }
        x_c=points[indexpoints];
        y_c=points[indexpoints+1];
        x_2=points[indexpoints+2];
        y_2=points[indexpoints+3];
        x3=points[indexpoints+4];
        y3=points[indexpoints+5];
        x4=points[indexpoints+6];
        y4=points[indexpoints+7];
        rr=points[indexpoints+8];
        c=points[indexpoints+9];
        num=points[indexpoints+10];
        indexpoints+=11;
        if(shape[i]=="line_DDA")
        {
            DDC(hdc,x_c,y_c,x_2,y_2);
        }
        else if(shape[i]=="line_midpoint")
        {
            midPoint(hdc,x_c,y_c,x_2,y_2);
        }
        else if(shape[i]=="line_parametric")
        {
            parametricline(hdc,x_c,y_c,x_2,y_2);

        }
        else if(shape[i]=="circle_direct")
        {
            CircleDirect(hdc,x_c,y_c,rr,num);

        }
        else if(shape[i]=="circle_polar")
        {
            CirclePolar(hdc,x_c,y_c,rr,num);

        }
        else if(shape[i]=="circle_iterative")
        {
            CirclePolar(hdc,x_c,y_c,rr,num);
        }
        else if(shape[i]=="circle_midpoint")
        {
            midpoint(hdc,x_c,y_c,rr,num);
        }
        else if(shape[i]=="circle_modified")
        {
            modfmidpoint(hdc,x_c,y_c,rr,num);

        }
        else if(shape[i]=="clipping_line")
        {
            Rectangle(hdc, x_c,y_c,x_2,y_2);
            CohenSuth(hdc,x3,y3,x4,y4,x_c,y_c,x_2,y_2);

        }
        else if(shape[i]=="clipping_point")
        {
            Rectangle(hdc, x_c,y_c,x_2,y_2);
            PointClipping(hdc,x3,y3,x_c,y_c,x_2,y_2);

        }
        else if(shape[i]=="ellipse_direct")
        {
            directEllipse(hdc, x_c, y_c, rr, 1.5);

        }
        else if(shape[i]=="ellipse_polar")
        {
            polarEllipse(hdc, x_c, y_c, x_2, y_2, 1.5);

        }
    }
}

/*  This function is called by the Windows function DispatchMessage()  */
int rr=0,x_c=0,y_c=0,x_2=0,y_2=0,c = 0;
int num;
int x3=0,y3=0,Num_of_Points=0;
int x4=0,y4=0;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDBLCLK:
            if(Num_of_Points==0)
            {
                x_c=LOWORD(lParam);
                y_c=HIWORD(lParam);
                Num_of_Points++;
            }
            else if(Num_of_Points == 1)
            {
                x_2=LOWORD(lParam);
                y_2=HIWORD(lParam);
                Num_of_Points++;
            }else if(Num_of_Points == 2){
                x3=LOWORD(lParam);
                y3=HIWORD(lParam);
                Num_of_Points++;
            }else if(Num_of_Points==3){
                x4=LOWORD(lParam);
                y4=HIWORD(lParam);
                Num_of_Points=0;
            }
            break;
        case WM_RBUTTONDBLCLK:
            if(Num_of_Points==1){
                x_2=LOWORD(lParam);
                y_2=HIWORD(lParam);
                Num_of_Points++;
            }
            else
            {
                x3=LOWORD(lParam);
                y3=HIWORD(lParam);
                Num_of_Points++;
            }


            break;
        case WM_COMMAND:
            switch(wParam)
            {
                //line
                case LINE_DDA:
                    DDC(hdc,x_c,y_c,x_2,y_2);
                    Num_of_Points=0;
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"line_DDA",num);
                    break;
                case LINE_MIDPOINT:
                    midPoint(hdc,x_c,y_c,x_2,y_2);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"line_midpoint",num);
                    Num_of_Points=0;
                    break;
                case LINE_PARAMETRIC:
                    parametricline(hdc,x_c,y_c,x_2,y_2);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"line_parametric",num);
                    Num_of_Points=0;
                    break;
                //circle
                case CIRCLE_DIRECT:
                    cout<<"Enter num: ";
                    cin>>num;
                    rr=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
                    CircleDirect(hdc,x_c,y_c,rr,num);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"circle_direct",num);
                    Num_of_Points=0;
                    break;
                case CIRCLE_POLAR:
                    cout<<"Enter num: ";
                    cin>>num;
                    rr=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
                    CirclePolar(hdc,x_c,y_c,rr,num);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"circle_polar",num);
                    Num_of_Points=0;
                    break;
                case CIRCLE_ITERATIVE:
                    cout<<"Enter num: ";
                    cin>>num;
                    rr=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
                    CircleIterativePolar(hdc,x_c,y_c,rr,num);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"circle_iterative",num);
                    Num_of_Points=0;
                    break;
                case CIRCLE_MIDPOINT:
                    cout<<"Enter num: ";
                    cin>>num;
                    rr=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
                    midpoint(hdc,x_c,y_c,rr,num);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"circle_midpoint",num);
                    Num_of_Points=0;
                    break;
                case CIRCLE_MODIFIED:
                    cout<<"Enter num: ";
                    cin>>num;
                    rr=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
                    modfmidpoint(hdc,x_c,y_c,rr,num);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"circle_modified",num);
                    Num_of_Points=0;
                    break;
                //cliping
                case CLIPPING_LINE:
                    Rectangle(hdc, x_c,y_c,x_2,y_2);
                    CohenSuth(hdc,x3,y3,x4,y4,x_c,y_c,x_2,y_2);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"clipping_line",num);
                    Num_of_Points=0;
                    break;
                case CLIPPING_POINT:

                    Rectangle(hdc, x_c,y_c,x_2,y_2);
                    PointClipping(hdc,x3,y3,x_c,y_c,x_2,y_2);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"clipping_point",num);
                    Num_of_Points=0;
                    break;
                //ellipse
                case ELLIPSE_DIRECT:
                    rr=sqrt(pow((x_c-x_2),2)+pow((y_c-y_2),2));
                    directEllipse(hdc, x_c, y_c, rr, 1.5);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"ellipse_direct",num);
                    Num_of_Points=0;
                    break;
                case ELLIPSE_POLAR:
                    polarEllipse(hdc, x_c, y_c, x_2, y_2, 1.5);
                    pushVectors(x_c,y_c,x_2,y_2,x3,y3,x4,y4,rr,c,"ellipse_polar",num);
                    Num_of_Points=0;
                    break;
                case RED:
                    cout<<"red\n";
                    color=RGB(255,0,0);
                    colorName="red";
                    break;
                case GREEN:
                    color=RGB(0,255,0);
                    colorName="green";

                    break;
                case BLUE:
                    color=RGB(0,0,255);
                    colorName="blue";
                    break;

                //save and load
                case SAVE:
                    save_toFile();
                    break;
                case LOAD:
                    load_fromFile();
                    drawAll(hdc);
                    break;

                case EXIT:
                    MessageBeep(MB_OK);
                    DestroyWindow(hwnd);
                    break;
                case CLEAR:
                    InvalidateRect(hwnd, NULL, TRUE);
                    points.clear();
                    shape.clear();
                    colors.clear();
                    Num_of_Points=0;
                    break;

            }
           // break;
        case WM_CREATE:
            addMenu(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

