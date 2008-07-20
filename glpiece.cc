/*
** -- Qt Includes --
*/

#include <QMouseEvent>

/*
** -- Local Includes --
*/

#include "maxit.h"
#include "glpiece.h"

glpiece::glpiece(QWidget *parent, glpiece *other,
		 const int valueArg, const QColor &bgColorArg,
		 const int rowArg, const int colArg, const int sideArg,
		 const int zRotArg):
  QGLWidget(parent, other),
  colv(colArg), rowv(rowArg), side(sideArg), valuev(valueArg),
  bgColor(bgColorArg), bgColorOrig(bgColorArg)
{
  xRot = 0;
  yRot = 0;
  zRot = zRotArg;
  consumed = false;
  setMouseTracking(true);
}

glpiece::~glpiece()
{
  glDeleteLists(piece, 1);
}

void glpiece::reset(const int valueArg)
{
  valuev = valueArg;
  consumed = false;
  setEnabled(true);

  if(side == 0)
    for(int i = 0; side < CUBE_SIZE; i++)
      {
	if(i % 15 == 0)
	  if(Global::maxitptr->getViewMode() == maxit::VIEW2D)
	    growBy(2);
	  else
	    growBy(4);

	if(Global::maxitptr->getViewMode() == maxit::VIEW2D)
	  rotateBy(0, 0, 5 * 10);
	else
	  rotateBy(5 * 10, -25 * 10, 5 * 10);
      }

  glDeleteLists(piece, 1);
  makeCurrent();
  resizeGL(width(), height());
  updateGL();
}

void glpiece::paintGL(void)
{
  qglClearColor(bgColor);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -10.0);
  glRotated(xRot / 64.0, 1.0, 0.0, 0.0);
  glRotated(yRot / 64.0, 0.0, 1.0, 0.0);
  glRotated(zRot / 64.0, 0.0, 0.0, 1.0);
  glDeleteLists(piece, 1);

  if(!consumed)
    {
      piece = createPiece();
      glCallList(piece);
    }
}

void glpiece::initializeGL(void)
{
  /*
  ** Initialize OpenGL.
  */

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
}

void glpiece::resizeGL(int w, int h)
{
  glViewport((w - side) / 2, (h - side) / 2, side, side);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, 0.5, 0.5, -0.5, 4.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}

void glpiece::rotate(const int xAngle, const int yAngle, const int zAngle)
{
  xRot = xAngle;
  yRot = yAngle;
  zRot = zAngle;
  updateGL();
}

void glpiece::rotateBy(const int xAngle, const int yAngle, const int zAngle)
{
  xRot += xAngle;
  yRot += yAngle;
  zRot += zAngle;
  updateGL();
}

void glpiece::growBy(const int growth)
{
  side += static_cast<int> (growth);
  resizeGL(width(), height());
}

void glpiece::shrinkBy(const int decrease)
{
  side -= static_cast<int> (decrease);
  resizeGL(width(), height());
}

GLuint glpiece::createPiece(void)
{
  GLuint list = glGenLists(1);
  GLuint textures[6];
  QString facevalue = "";
  static const int coords[6][4][3] =
    {
      {{1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}},
      {{1, 1, -1}, {-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}},
      {{1, -1, 1}, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}},
      {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}},
      {{1, -1, 1}, {-1, -1, 1}, {-1, -1, -1}, {1, -1, -1}},
      {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}}
    };

  if(valuev < 0)
    facevalue = "q";
  else
    facevalue = QString::number(valuev);

  for(int i = 0; i < 6; i++)
    textures[i] = bindTexture
      (QPixmap(QString("images.d/ubuntu.d/%1.png").arg(facevalue)),
       GL_TEXTURE_2D);

  glNewList(list, GL_COMPILE);

  for(int i = 0; i < 6; i++)
    {
      glBindTexture(GL_TEXTURE_2D, textures[i]);
      glBegin(GL_QUADS);

      for(int j = 0; j < 4; j++)
	{
	  glTexCoord2d(j == 0 || j == 3, j == 0 || j == 1);
	  glVertex3d(0.21 * coords[i][j][0], 0.21 * coords[i][j][1],
		     0.21 * coords[i][j][2]);
	}

      glEnd();
    }

  glEndList();
  return list;
}

void glpiece::enterEvent(QEvent *e)
{
  (void) e;

  if(consumed || !isEnabled())
    return;

  /*
  ** Highlight the piece.
  */

  if(bgColor == Qt::black)
    bgColor = Qt::gray;
  else
    bgColor = QColor
      (abs(static_cast<int> (bgColor.red() + bgColor.red() * 0.50)),
       abs(static_cast<int> (bgColor.green() + bgColor.green() * 0.50)),
       abs(static_cast<int> (bgColor.blue() + bgColor.blue() * 0.50)));

  updateGL();
}

void glpiece::leaveEvent(QEvent *e)
{
  (void) e;

  if(consumed || !isEnabled())
    return;

  bgColor = bgColorOrig;
  updateGL();
}

void glpiece::select(void)
{
  mousePressEvent(static_cast<QMouseEvent *> (0));
}

void glpiece::mousePressEvent(QMouseEvent *e)
{
  (void) e;

  if(consumed)
    return;

  /*
  ** Shrink and spin the piece.
  */

  for(int i = 0; side > 0; i++)
    {
      if(i % 15 == 0)
	if(Global::maxitptr->getViewMode() == maxit::VIEW2D)
	  shrinkBy(2);
	else
	  shrinkBy(4);

      if(Global::maxitptr->getViewMode() == maxit::VIEW2D)
	rotateBy(0, 0, -5 * 10);
      else
	rotateBy(-5 * 10, 25 * 10, -5 * 10);
    }

  glDeleteLists(piece, 1);
  consumed = true;
  bgColor = bgColorOrig;
  updateGL();

  if(e) // Ignore non-user calls.
    Global::maxitptr->pieceSelected(this);
}

QSize glpiece::minimumSizeHint(void) const
{
  return QSize
    (side - static_cast<int> (side * 0.5),
     side - static_cast<int> (side * 0.5));
}

QSize glpiece::sizeHint(void) const
{
  return QSize(side + static_cast<int> (side * 0.5),
	       side + static_cast<int> (side * 0.5));
}

int glpiece::col(void) const
{
  return colv;
}

int glpiece::row(void) const
{
  return rowv;
}

int glpiece::value(void) const
{
  return abs(valuev);
}

void glpiece::setEnabled(const bool state)
{
  static_cast<QGLWidget *> (this)->setEnabled(state);
  bgColor = bgColorOrig;

  if(!isEnabled())
    bgColor = QColor
      (abs(static_cast<int> (bgColor.red() - bgColor.red() * 0.50)),
       abs(static_cast<int> (bgColor.green() - bgColor.green() * 0.50)),
       abs(static_cast<int> (bgColor.blue() - bgColor.blue() * 0.50)));

  updateGL();
}

void glpiece::setValue(const int valueArg)
{
  valuev = valueArg;
}
