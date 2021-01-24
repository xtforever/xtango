/*    Viewing Transformations
         by Kim Liu


Description:

  The program "vt" uses the XTANGO Algorithm Animation Designer's Package to
provide a visual representation of the application of the parallel perspective
viewing transformation.  The representation consists of a representation of
the view point and view axis, the model space origin and axis, and a sample
object in model space; a square.
  The program reads in a data file which specifies the position of the square
in the model space, the position of the view point in model space, and the
position of the XTANGO view point; that is, the view seen from the XTANGO
window of the transformations occuring in the model space.
  When run, the program displays the sqaure, the model space origin and axis,
and the view point and view axis.  The square is in blue, the model origin in
maroon, and the view point in black.  The arrowhead on one of the axis of
both the model and view axis indicates the relative Z axis for each.
  The program first performs a translation which moves the coordinates for the
viewing plane of the view point to the origin. This is followed by a
rotation which maps the coordinate axis of the model space into the view space,
so that the view space is the reference coordinate system.  The view point is
moved to the origin, then the image is sheared and scaled along the direction
of view.

Problems/Enhancements:
  The program currently assumes that the user has some idea of what the
viewing transformation is used for, (mapping a three dimensional object to a
two dimensional window.)  A more discriptive display, one which shows the
reasons for the actions while they are occuring would be better.  This would
include showing the direction of view from the view point, the viewing plane
and target window, and perhaps a second display showing the view from the
view point.
  A better description of the data file is needed.
  A mechanism to allow the user to pause the animation AND change the view
point of the animation to watch the occurences from a different perspective.
  This program shows the parallel projection; to do the perspective projection
in an understandable fashion would require the additional visual objects
mentioned above to see how the view point and view window interact with regards
to scaling the model space to the perspective projection.

*/


#include <stdio.h>
#include <memory.h>
#include <xtango.h>
#include <math.h>
#include "viewtran.h"

/*\
|*| Structures, externs, defines, and globals
\*/
line_t ViewOrigin[3]; /* One line segment per axis */
line_t ModelOrigin[3]; /* One line segment per axis */
line_t SquareL[4]; /* One line segment per edge */
polyline_t ViewOrg;
polyline_t ModelOrg;
polyline_t Square;
view_t TangoView;
view_t SampleView;

void WorldToWindow();
void CreateView();
void IDMatrix();
void NormalizeVector();
void TransformPoint();
void TransformPolyline();
void MatrixTranslate();
void CrossProduct();
void MatrixMultiply();




/*\
|*| XTango Stuff
\*/

void ANIMInitializeImages();
void ANIMmakeViewTrans( );
void ANIMmakeModelTrans( );
void ANIMmakeSquareTrans( );
void ANIMturnon( );
void ANIMdoTrans( );
void ANIMprint();
void ANIMunprint();
void ANIMcleanup();
static NAME_FUNCT namefun[] ={{"Init I",1,{VOID,(FPTR)ANIMInitializeImages}},
                              {"makeVT",1, {VOID, (FPTR) ANIMmakeViewTrans}},
                              {"makeMT",1, {VOID, (FPTR) ANIMmakeModelTrans}},
                              {"makeST",1, {VOID, (FPTR) ANIMmakeSquareTrans}},
                              {"turnon",1,{VOID, (FPTR) ANIMturnon}},
                              {"doT"  ,1, {VOID, (FPTR) ANIMdoTrans}},
                              {"print",1,{VOID, (FPTR) ANIMprint}},
                              {"unprint",1,{VOID, (FPTR) ANIMunprint}},
                              {"cleanup",1,{VOID, (FPTR) ANIMcleanup}},
                              { NULL,       0, NULL, NULL} };

void InitializeObjects()
{
int Index;
 for (Index=0; Index<3; Index++) {
  ViewOrigin[Index].Start.X = 0.0; ViewOrigin[Index].Start.Y = 0.0;
  ViewOrigin[Index].Start.Z = 0.0;
  ModelOrigin[Index].Start.X = 0.0; ModelOrigin[Index].Start.Y = 0.0;
  ModelOrigin[Index].Start.Z = 0.0;
  ViewOrigin[Index].End.X = 0.0; ViewOrigin[Index].End.Y = 0.0;
  ViewOrigin[Index].End.Z = 0.0;
  ModelOrigin[Index].End.X = 0.0; ModelOrigin[Index].End.Y = 0.0;
  ModelOrigin[Index].End.Z = 0.0;
 }
 ViewOrigin[0].End.X = 1.0;
 ViewOrigin[1].End.Y = 1.0;
 ViewOrigin[2].End.Z = 1.0;
 ModelOrigin[0].End.X = 1.0;
 ModelOrigin[1].End.Y = 1.0;
 ModelOrigin[2].End.Z = 1.0;
 SquareL[ 0].Start.X = -0.5;SquareL[ 0].Start.Y = -0.5;SquareL[ 0].Start.Z = 0.0;
 SquareL[ 0].End.X = 0.5;  SquareL[ 0].End.Y = -0.5;  SquareL[ 0].End.Z = 0.0;  
 SquareL[ 1].Start.X = 0.5;SquareL[ 1].Start.Y = -0.5;SquareL[ 1].Start.Z = 0.0; 
 SquareL[ 1].End.X = 0.5;  SquareL[ 1].End.Y = 0.5;  SquareL[ 1].End.Z = 0.0;  
 SquareL[ 2].Start.X = 0.5;SquareL[ 2].Start.Y = 0.5;SquareL[ 2].Start.Z = 0.0;
 SquareL[ 2].End.X = -0.5;  SquareL[ 2].End.Y = 0.5;  SquareL[ 2].End.Z = 0.0;  
 SquareL[ 3].Start.X = -0.5;SquareL[ 3].Start.Y = 0.5;SquareL[ 3].Start.Z = 0.0;
 SquareL[ 3].End.X = -0.5;  SquareL[ 3].End.Y = -0.5;  SquareL[ 3].End.Z = 0.0;  

 ViewOrg.Count = 3;
 ModelOrg.Count = 3;
 Square.Count = 4;
 ViewOrg.Vertices = ViewOrigin;
 ModelOrg.Vertices = ModelOrigin;
 Square.Vertices = SquareL;
 TANGOalgoOp( namefun,"Init I" );
} /* End of InitializeObjects() */

void ReadInVector( In, Pnt )
FILE *In;
pnt_t *Pnt;
{
char line[80];
 fgets( line, 79, In );
 if (feof(In)) {
  fprintf(stderr, "Unexpected end of file.\n");
  exit(-1);
 } else {}
 sscanf( line, "%lf %lf %lf", &Pnt->X, &Pnt->Y, &Pnt->Z );
} /* End of ReadInVector */

void ReadInFile( FName )
char * FName;
{
 FILE *In;
 pnt_t Move;
 matrix_t Mat;
 matrix_t Mat1;

 In = fopen( FName, "r" );
 if (NULL==In) {
  fprintf(stderr, "Could not open input file!\n");
  exit(-1);
 } else {}

/*\
|*| Move cube around in model space
\*/
 ReadInVector( In, &Move );
 IDMatrix( Mat );
 MatrixTranslate( Mat, &Move );

/*\
|*| Read in the Tango window view
\*/
 ReadInVector( In, &TangoView.PRP );
 ReadInVector( In, &TangoView.VRP );
 ReadInVector( In, &TangoView.VUP );
 ReadInVector( In, &TangoView.VPN );
 fscanf( In, "%lf %lf %lf %lf\n",
   &TangoView.Window.Corner1.X,  &TangoView.Window.Corner1.Y,  
   &TangoView.Window.Corner2.X,  &TangoView.Window.Corner2.Y );  
 TangoView.Width = 1.0;
 TangoView.Height = 1.0;
 CreateView( &TangoView );

/*\
|*| Read in the Sample window view
\*/
 ReadInVector( In, &SampleView.PRP );
 ReadInVector( In, &SampleView.VRP );
 ReadInVector( In, &SampleView.VUP );
 ReadInVector( In, &SampleView.VPN );
 fscanf( In, "%lf %lf %lf %lf",
   &SampleView.Window.Corner1.X,  &SampleView.Window.Corner1.Y,  
   &SampleView.Window.Corner2.X,  &SampleView.Window.Corner2.Y );  
 fclose(In);

/*\
|*| Calculate the position and orientation of the viewpoint in model
|*| space.
\*/
 Move.X = -SampleView.VRP.X;
 Move.Y = -SampleView.VRP.Y;
 Move.Z = -SampleView.VRP.Z;
 memcpy( &ViewOrigin[2].End, &SampleView.VPN, sizeof( vector_t ));
 NormalizeVector( &ViewOrigin[2].End );
 CrossProduct( &SampleView.VUP, &ViewOrigin[2].End, &ViewOrigin[0].End );
 NormalizeVector( &ViewOrigin[0].End );
 CrossProduct( &ViewOrigin[2].End, &ViewOrigin[0].End, &ViewOrigin[1].End );
 NormalizeVector( &ViewOrigin[1].End );

 IDMatrix( Mat );
 MatrixTranslate( Mat, &Move );
 Move.X = (-1.0 * SampleView.PRP.X * ViewOrigin[0].End.X) +
          (-1.0 * SampleView.PRP.Y * ViewOrigin[0].End.Y) +
          (-1.0 * SampleView.PRP.Z * ViewOrigin[0].End.Z);
 Move.Y = (-1.0 * SampleView.PRP.X * ViewOrigin[1].End.X) +
          (-1.0 * SampleView.PRP.Y * ViewOrigin[1].End.Y) +
          (-1.0 * SampleView.PRP.Z * ViewOrigin[1].End.Z);
 Move.Z = (-1.0 * SampleView.PRP.X * ViewOrigin[2].End.X) +
          (-1.0 * SampleView.PRP.Y * ViewOrigin[2].End.Y) +
          (-1.0 * SampleView.PRP.Z * ViewOrigin[2].End.Z);
 IDMatrix( Mat1 );
 MatrixTranslate( Mat1, &Move );
 TransformPolyline( Mat, &ViewOrg );
 TransformPolyline( Mat1, &ViewOrg );
} /* End of ReadInFile */

/*\
|*| Draw ViewOrg
\*/
void DrawViewOrg( V, PL, Vis )
view_t *V;
polyline_t *PL;
int  Vis;
{
int Index;
double vx[7], vy[7];
line_t L[7];

 for(Index=0;Index<PL->Count;Index++){
  memcpy(&L[Index], &PL->Vertices[Index], sizeof(line_t));
  WorldToWindow( V, &L[Index].Start );
  WorldToWindow( V, &L[Index].End );
  L[Index].Start.Y *= -1.0;
  L[Index].End.Y *= -1.0;
  L[Index].Start.X += 0.5; L[Index].Start.Y += 0.5;
  L[Index].End.X += 0.5; L[Index].End.Y += 0.5;
 }
 vx[0] = L[0].Start.X;
 vx[1] = L[0].End.X - vx[0];
 vx[2] = L[1].End.X - vx[0];
 vx[3] = L[2].End.X - vx[0];
 vy[0] = L[0].Start.Y;
 vy[1] = L[0].End.Y - vy[0];
 vy[2] = L[1].End.Y - vy[0];
 vy[3] = L[2].End.Y - vy[0];
 TANGOalgoOp( namefun,"makeVT", vx, vy );
} /* End of DrawViewOrg */

void DrawModelOrg( V, PL, Vis )
view_t *V;
polyline_t *PL;
int  Vis;
{
int Index;
double vx[7], vy[7];
line_t L[7];

 for(Index=0;Index<PL->Count;Index++){
  memcpy(&L[Index], &PL->Vertices[Index], sizeof(line_t));
  WorldToWindow( V, &L[Index].Start );
  WorldToWindow( V, &L[Index].End );
  L[Index].Start.Y *= -1.0;
  L[Index].End.Y *= -1.0;
  L[Index].Start.X += 0.5; L[Index].Start.Y += 0.5;
  L[Index].End.X += 0.5; L[Index].End.Y += 0.5;
 }
 vx[0] = L[0].Start.X;
 vx[1] = L[0].End.X - vx[0];
 vx[2] = L[1].End.X - vx[0];
 vx[3] = L[2].End.X - vx[0];
 vy[0] = L[0].Start.Y;
 vy[1] = L[0].End.Y - vy[0];
 vy[2] = L[1].End.Y - vy[0];
 vy[3] = L[2].End.Y - vy[0];
 TANGOalgoOp( namefun,"makeMT", vx, vy );
} /* End of DrawModelOrg */

void DrawSquare( V, PL, Vis )
view_t *V;
polyline_t *PL;
int  Vis;
{
int Index;
double vx[4], vy[4];
line_t L[4];

 for(Index=0;Index<PL->Count;Index++){
  memcpy(&L[Index], &PL->Vertices[Index], sizeof(line_t));
  WorldToWindow( V, &L[Index].Start );
  WorldToWindow( V, &L[Index].End );
  L[Index].Start.Y *= -1.0;
  L[Index].End.Y *= -1.0;
  L[Index].Start.X += 0.5; L[Index].Start.Y += 0.5;
  L[Index].End.X += 0.5; L[Index].End.Y += 0.5;
 }
 vx[0] = L[0].Start.X;
 vx[1] = L[1].Start.X;
 vx[2] = L[2].Start.X;
 vx[3] = L[3].Start.X;
 vy[0] = L[0].Start.Y;
 vy[1] = L[1].Start.Y;
 vy[2] = L[2].Start.Y;
 vy[3] = L[3].Start.Y;
 TANGOalgoOp( namefun,"makeST", vx, vy );
} /* End of DrawSquare */


int main( argc, argv )
int argc;
char **argv;
{
matrix_t Mat1;
double dx, dy;

/*\
|*| Initialization
|*|   Create objects
\*/
 printf("Illustrating viewing transformations\n\n");

 InitializeObjects();

/* InitializeTango */
 TANGOalgoOp( namefun, "BEGIN" );

/*\
|*| Read in values
|*|   Set up objects
\*/
 if (2>argc) {
  fprintf(stderr, "Please specify data file on command line.\n");
  fprintf(stderr, "viewtran.in will work as an example.\n");
  exit(0);
 } else {
  ReadInFile( argv[1] );
 } 

  TANGOalgoOp( namefun, "print", "Display Objects" );

/*\
|*| Draw respective origins and sample object
|*|   Need function to render each
\*/
  DrawViewOrg( &TangoView, &ViewOrg, 0 );
  DrawModelOrg( &TangoView, &ModelOrg, 0 );
  DrawSquare( &TangoView, &Square,0 );
  TANGOalgoOp( namefun,"doT", 0 );

  TANGOalgoOp( namefun,"turnon" );

  TANGOalgoOp( namefun, "unprint" );
  TANGOalgoOp( namefun, "print", "Move VRP of view point to the origin." );
/*\
|*| Move to Origin
\*/
 {
  matrix_t Matrix;
  IDMatrix( Matrix );
  MatrixTranslate( Matrix, &SampleView.VRP );
  TransformPolyline( Matrix, &ModelOrg );
  TransformPolyline( Matrix, &ViewOrg );
  TransformPolyline( Matrix, &Square );
  DrawModelOrg( &TangoView, &ModelOrg,1 );
  DrawViewOrg( &TangoView, &ViewOrg,1 );
  DrawSquare( &TangoView, &Square,1 );
  TANGOalgoOp( namefun,"doT", 1 );
 }

  TANGOalgoOp( namefun, "unprint" );
  TANGOalgoOp( namefun, "print", "Rotate the coordinate axis to match the view point.");
/*\
|*| Rotate axis
\*/
 {
  vector_t NewX; vector_t NewY; vector_t NewZ;
  matrix_t Matrix;
  IDMatrix( Matrix );
  memcpy( &NewZ, &SampleView.VPN, sizeof( vector_t ));
  NormalizeVector( &NewZ );
  memcpy( &NewY, &SampleView.VUP, sizeof( vector_t ));
  CrossProduct( &NewY, &NewZ, &NewX );
  NormalizeVector( &NewX );
  CrossProduct( &NewZ, &NewX, &NewY );
  NormalizeVector( &NewY );
  Matrix[ 0][ 0]= NewX.X;  Matrix[ 1][ 0]= NewX.Y;  Matrix[ 2][ 0]= NewX.Z;  
  Matrix[ 0][ 1]= NewY.X;  Matrix[ 1][ 1]= NewY.Y;  Matrix[ 2][ 1]= NewY.Z;  
  Matrix[ 0][ 2]= NewZ.X;  Matrix[ 1][ 2]= NewZ.Y;  Matrix[ 2][ 2]= NewZ.Z;  
  TransformPolyline( Matrix, &ModelOrg );
  TransformPolyline( Matrix, &ViewOrg );
  TransformPolyline( Matrix, &Square );
  DrawViewOrg( &TangoView, &ViewOrg,1 );
  DrawSquare( &TangoView, &Square,1 );
  DrawModelOrg( &TangoView, &ModelOrg,1 );
  TANGOalgoOp( namefun,"doT", 1 );
 }

  TANGOalgoOp( namefun, "unprint" );
  TANGOalgoOp( namefun, "print", "Move the PRP of the view point to the origin.");
/*\
|*| Move PRP to Origin
\*/
 {
  matrix_t Matrix;
  IDMatrix( Matrix );
  MatrixTranslate( Matrix, &SampleView.PRP );
  TransformPolyline( Matrix, &ViewOrg );
  TransformPolyline( Matrix, &ModelOrg );
  TransformPolyline( Matrix, &Square );
  DrawViewOrg( &TangoView, &ViewOrg,1 );
  DrawModelOrg( &TangoView, &ModelOrg,1 );
  DrawSquare( &TangoView, &Square,1 );
  TANGOalgoOp( namefun,"doT", 1 );
 }

  TANGOalgoOp( namefun, "unprint" );
  TANGOalgoOp( namefun, "print", "Apply shear for parallel projection" );
/*\
|*| Apply Shear 
\*/
 {
  vector_t Shear;
  matrix_t Matrix;
  IDMatrix( Matrix );
  Shear.X = (SampleView.Window.Corner2.X + SampleView.Window.Corner1.X);
  Shear.Y = (SampleView.Window.Corner2.Y + SampleView.Window.Corner1.Y);
  Shear.Z = 0;
  Shear.X -= SampleView.PRP.X;
  Shear.Y -= SampleView.PRP.Y;
  Shear.Z -= SampleView.PRP.Z;
  if (0.0==Shear.Z) {
   fprintf(stderr, "Bad value for PRP z value!  Cannot divide by zero.\n");
   exit(-20);
  } else{}
  Matrix[0][2]= -Shear.X/Shear.Z;
  Matrix[1][2]= -Shear.Y/Shear.Z;
  TransformPolyline( Matrix, &Square );
  TransformPolyline( Matrix, &ViewOrg );
  TransformPolyline( Matrix, &ModelOrg );
  DrawSquare( &TangoView, &Square,1 );
  DrawViewOrg( &TangoView, &ViewOrg,1 );
  DrawModelOrg( &TangoView, &ModelOrg,1 );
  TANGOalgoOp( namefun,"doT", 1 );
 }

  TANGOalgoOp( namefun, "unprint" );
  TANGOalgoOp( namefun, "print", "Scale the image" );
/*\
|*| Scale
\*/
 {
  vector_t Vec;
  matrix_t Matrix1;
  matrix_t Matrix2;
  matrix_t Matrix3;
  IDMatrix( Matrix1 );
  IDMatrix( Matrix2 );
  Vec.X = (SampleView.Window.Corner2.X + SampleView.Window.Corner1.X)
           -SampleView.PRP.X;
  Vec.Y = (SampleView.Window.Corner2.Y + SampleView.Window.Corner1.Y)
           -SampleView.PRP.Y;
  Vec.Z = -SampleView.PRP.Z;
  if (0.0==Vec.Z) {
   fprintf(stderr, "Zero distance between view point and view plane. Abort.\n");
   exit(1);
  } else {}
  Matrix1[2][0] = Vec.X/SampleView.PRP.Z;
  Matrix1[2][1] = Vec.Y/SampleView.PRP.Z;
  Vec.X = 2.0/(SampleView.Window.Corner2.X - SampleView.Window.Corner1.X);
  Vec.Y = 2.0/(SampleView.Window.Corner2.Y - SampleView.Window.Corner1.Y);
  Vec.Z = 1.0;
  Matrix2[0][0]= Vec.X;
  Matrix2[1][1]= Vec.Y;
  MatrixMultiply(Matrix1, Matrix2, Matrix3 );
  TransformPolyline( Matrix3, &Square );
  TransformPolyline( Matrix3, &ViewOrg );
  TransformPolyline( Matrix3, &ModelOrg );
  DrawSquare( &TangoView, &Square ,1);
  DrawViewOrg( &TangoView, &ViewOrg,1 );
  DrawModelOrg( &TangoView, &ModelOrg,1 );
  TANGOalgoOp( namefun,"doT", 1 );
 }

/*\
|*| Done
\*/
 TANGOalgoOp( namefun, "unprint" );
 TANGOalgoOp( namefun, "print", "Parallel perspective viewing transformation done." );
 TANGOalgoOp( namefun, "END" );
 return 0;
} /* End of main */





void printpnt( p )
pnt_t *p;
{
fprintf(stdout, "%lf %lf %lf pnt\n", p->X, p->Y, p->Z );
}

void printmat( m )
matrix_t m;
{
fprintf(stdout, "%lf %lf %lf %lf\n", m[0][0], m[1][0], m[2][0], m[3][0] );
fprintf(stdout, "%lf %lf %lf %lf\n", m[0][1], m[1][1], m[2][1], m[3][1] );
fprintf(stdout, "%lf %lf %lf %lf\n", m[0][2], m[1][2], m[2][2], m[3][2] );
fprintf(stdout, "%lf %lf %lf %lf\n", m[0][3], m[1][3], m[2][3], m[3][3] );
fprintf(stdout, "\n");
} /* End of printmat */

void WorldToView( V, P )
view_t *V; pnt_t *P;
{
  TransformPoint( V->View, P );
} /* End of WorldToView */

void WorldToPRP( V, P )
view_t *V; pnt_t *P;
{
  TransformPoint( V->Rigid, P );
} /* End of WorldToPRP */

void ViewToWindow( V, P )
view_t *V; pnt_t *P;
{
pnt_t Tmp;
  memcpy( &Tmp, P, sizeof( pnt_t ));
  P->X = Tmp.X * V->Distance/Tmp.Z;
  P->Y = Tmp.Y * V->Distance/Tmp.Z;
  P->Z = (Tmp.Z - 0.0000000001)/(Tmp.Z*(1.0000000001));
} /* End of ViewToWindow */

void WorldToWindow( V, P )
view_t *V; pnt_t *P;
{
  WorldToView( V, P );
  ViewToWindow( V, P );
} /* End of WorldToWindow */

void CreateView( V )
view_t *V;
{
pnt_t *PRP = &(V->PRP);
pnt_t *VUP = &(V->VUP);
pnt_t *VPN = &(V->VPN);
pnt_t *VRP = &(V->VRP);
vector_t V1;
vector_t V2;
vector_t V3;
matrix_t M1;
matrix_t M2;
matrix_t M3;

  IDMatrix( M1 );
  IDMatrix( M2 );
  MatrixTranslate( M1, VRP );
  memcpy( &V1, VPN, sizeof( pnt_t ) ) ;
  NormalizeVector( &V1 );
  M2[0][2]= V1.X;
  M2[1][2]= V1.Y;
  M2[2][2]= V1.Z;
  CrossProduct( VUP, &V1, &V2 );
  NormalizeVector( &V2 );
  M2[0][0]= V2.X;
  M2[1][0]= V2.Y;
  M2[2][0]= V2.Z;
  CrossProduct( &V1, &V2, &V3 );
  NormalizeVector( &V3 );
  M2[0][1]= V3.X;
  M2[1][1]= V3.Y;
  M2[2][1]= V3.Z;
  MatrixMultiply( M2, M1, M3 );
  memcpy( &V1, PRP, sizeof( pnt_t ) ) ;
/*
  TransformVector( M3, &V1 );
  TransformVector( M3, PRP );
*/
  IDMatrix( M1 );
  MatrixTranslate( M1, &V1 );
  MatrixMultiply( M1, M3, V->Rigid );
  V1.X = (V->Window.Corner1.X + V->Window.Corner2.X) - PRP->X;
  V1.Y = (V->Window.Corner1.Y + V->Window.Corner2.Y) - PRP->Y;
  V->Distance = V1.Z = -(PRP->Z);
  if (!V1.Z) {
    fprintf(stderr, "Zero distance between the view point and the view plane. Fatal Error.\n\n");
    exit(-1);
  } else { }
  IDMatrix( M1 );
  M1[2][0] = -1.0 * V1.X /V1.Z;
  M1[2][1] = -1.0 * V1.Y /V1.Z;
  MatrixMultiply( M1, V->Rigid, M3 );
  IDMatrix( M2 );
  M2[0][0] = 2.0/(V->Width);
  M2[1][1] = 2.0/(V->Height);
  M2[2][2] = -1.0/(V1.Z);
  MatrixMultiply( M2, M3, V->View );
} /* End of Create View */



void NullMatrix( matrix )
matrix_t matrix;
{
int X,Y;
  for (X=0; X<4;X++) for (Y=0;Y<4;Y++) matrix[X][Y]=0.0;
} /* End of Null Matrix */

void IDMatrix( matrix )
matrix_t matrix;
{
  NullMatrix( matrix );
  matrix[0][0] = 1.0;
  matrix[1][1] = 1.0;
  matrix[2][2] = 1.0;
  matrix[3][3] = 1.0;
} /* End of Null Matrix */

void NormalizeVector( vector )
vector_t * vector;
{
double Length;
  Length = vector->X * vector->X +
           vector->Y * vector->Y +
           vector->Z * vector->Z ;
  if (Length > 0) Length = sqrt( Length );
  vector->X /= Length;
  vector->Y /= Length;
  vector->Z /= Length;
}

void TransformPoint( matrix, point )
matrix_t matrix; pnt_t * point;
{
pnt_t Temp;
   memcpy( &Temp, point, sizeof( pnt_t ) );
   point->X = matrix[0][0]*Temp.X + matrix[1][0] * Temp.Y +
              matrix[2][0]*Temp.Z + matrix[3][0];
   point->Y = matrix[0][1]*Temp.X + matrix[1][1] * Temp.Y +
              matrix[2][1]*Temp.Z + matrix[3][1];
   point->Z = matrix[0][2]*Temp.X + matrix[1][2] * Temp.Y +
              matrix[2][2]*Temp.Z + matrix[3][2];
} /* End of TransformPoint */

void TransformLine( matrix, line )
matrix_t matrix; line_t *line;
{
  TransformPoint( matrix, &line->Start );
  TransformPoint( matrix, &line->End   );
} /* End of TransformLine */

void TransformVector( matrix, vector )
matrix_t matrix; vector_t *vector;
{
  TransformPoint( matrix, (pnt_t *) vector );
} /* End of TransformVector */

void TransformRay( matrix, ray )
matrix_t matrix; ray_t * ray;
{
  TransformPoint( matrix, &ray->Start );
  TransformVector( matrix, &ray->Vector   );
} /* End of TransformRay */

void TransformRectangle( matrix, rect )
matrix_t matrix; rectangle_t * rect;
{
  TransformPoint( matrix, &rect->Corner1 );
  TransformPoint( matrix, &rect->Corner2 );
} /* End of TransformRectangle */

void TransformPolyline( matrix, poly )
matrix_t matrix; polyline_t *poly;
{
int Index;

  for (Index =0; Index<poly->Count; Index ++)
    TransformLine( matrix, &poly->Vertices[Index] );
} /* End of TransformPolyline */

void TransformMesh( matrix, mesh )
matrix_t matrix; mesh_t * mesh;
{
int IndexX;
int IndexY;
int Width;
  Width = mesh->Width;
  for (IndexX =Width; IndexX ; ) {
    IndexX --;
    for (IndexY =mesh->Height; IndexY  ; ) {
      IndexY --;
      TransformPoint( matrix, &mesh->Points[IndexY*Width+IndexX] );
    }
  }
} /* End of TransformMesh */

void MatrixTranslate( matrix, pnt )
matrix_t matrix; pnt_t * pnt;
{
  matrix[3][0] -= pnt->X;
  matrix[3][1] -= pnt->Y;
  matrix[3][2] -= pnt->Z;
} /* End of MatrixTranslate */

void CrossProduct( A, B, C )
vector_t *A; vector_t *B; vector_t *C;
{
vector_t Temp;
  Temp.X = A->Y * B->Z - A->Z * B->Y;
  Temp.Y = A->Z * B->X - A->X * B->Z;
  Temp.Z = A->X * B->Y - A->Y * B->X;
  memcpy( C, &Temp, sizeof( vector_t ));
} /* End of CrossProduct */

double DotProduct( A, B )
vector_t *A; vector_t *B;
{
  return A->X * B->X + A->Y * B->Y + A->Z * B->Z ;
} /* End of DotProduct */

void MatrixMultiply( M1, M2, M3 )
matrix_t M1, M2, M3;
{
int X,Y,Z;
  NullMatrix( M3 );
  for (X=0;X<4;X++) for(Y=0;Y<4;Y++) for(Z=0;Z<4;Z++)
    M3[X][Y] += M1[Z][Y]*M2[X][Z];
} /* MatrixMultiply */



