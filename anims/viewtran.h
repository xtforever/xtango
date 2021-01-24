/*\
|*| GR.H -- some simple, generally useful definitions
\*/

typedef struct {
  double Red;
  double Blue;
  double Green;
  } color_t;

typedef struct {
  double X;
  double Y;
  double Z;
  } pnt_t;

typedef pnt_t vector_t;

typedef struct {
  pnt_t Start;
  pnt_t End;
  } line_t;

typedef struct {
  pnt_t Start;
  vector_t Vector;
  } ray_t;

typedef struct {
  pnt_t Corner1;
  pnt_t Corner2;
  } rectangle_t;

typedef struct {
  int    Count;
  line_t *Vertices;
  } polyline_t;

typedef struct {
  int    Width;
  int    Height;
  pnt_t *Points;
  } mesh_t;

typedef double matrix_t[4][4];

typedef struct {
  pnt_t       PRP;
  pnt_t       VUP;
  pnt_t       VPN;
  pnt_t       VRP;
  rectangle_t Window;
  double      Distance;
  matrix_t    Rigid;
  matrix_t    View;
  int         Width;
  int         Height;
} view_t;


/*
void NullMatrix( matrix_t );
void IDMatrix( matrix_t );
void NormalizeVector( vector_t * );
void TransformPoint( matrix_t , pnt_t * );
void TransformLine( matrix_t , line_t * );
void TransformVector( matrix_t , vector_t * );
void TransformRay( matrix_t , ray_t * );
void TransformRectangle( matrix_t , rectangle_t * );
void TransformPolyline( matrix_t , polyline_t * );
void TransformMesh( matrix_t , mesh_t * );
void MatrixTranslate( matrix_t , pnt_t *);
void CrossProduct( vector_t * , vector_t * , vector_t * );
double DotProduct( vector_t * , vector_t * );
void MatrixMultiply( matrix_t , matrix_t , matrix_t );
void WorldToView( view_t *, pnt_t *);
void WorldToPRP( view_t *, pnt_t *);
void ViewToWindow( view_t *, pnt_t *);
void WorldToWindow( view_t *, pnt_t *);
void CreateView( view_t *);

*/
