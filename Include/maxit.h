#ifndef _MAXIT_
#define _MAXIT_

/*
** -- C Includes --
*/

#include <math.h>

/*
** -- C++ Includes --
*/

#include <iostream>

/*
** -- Qt Includes --
*/

#include <QMainWindow>
#include <QMessageBox>

/*
** -- Local Includes --
*/

#include "common.h"
#include "computer.h"
#include "glpiece.h"
#include "ui_maxit.h"

class maxit: public QMainWindow, private Ui::maxit_window
{
  Q_OBJECT

 public:
  enum
  {
    VIEW2D = 0,
    VIEW3D = 1
  };

  maxit(void);
  bool isAnimationEnabled(void) const;
  int getViewMode(void) const;
  int getViewSize(void) const;
  void pieceSelected(glpiece *);
  static QString themedir(void);

 private:
  QAction *action_2D;
  QAction *action_3D;
  QAction *action_4x4;
  QAction *action_5x5;
  QAction *action_6x6;
  QAction *action_7x7;
  QAction *action_difficult;
  QAction *action_easy;
  QAction *action_normal;
  QGridLayout *qgl;
  glpiece *computerlastpiece;
  glpiece *glpieces[Global::NROWS][Global::NCOLS];
  int size;
  static QString themepath;
  void prepareBoard(const bool = true);

 private slots:
  void slotAbout(void);
  void slotChangeDifficulty(void);
  void slotChangeSize(void);
  void slotChangeTheme(void);
  void slotChangeView(void);
  void slotInstructions(void);
  void slotNewGame(void);
  void slotShowHint(void);
};

#endif
