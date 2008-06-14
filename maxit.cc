#include "maxit.h"

extern QApplication *qapp;

maxit::maxit(void):QMainWindow()
{
  int i = 0;
  int j = 0;
  int value = 1;
  QColor color;
  QGridLayout *qgl = NULL;

  setupUi(this);
  connect((QObject *) action_Exit, SIGNAL(triggered(void)),
	  qapp, SLOT(quit(void)));
  connect((QObject *) action_About, SIGNAL(triggered(void)),
	  this, SLOT(slotAbout(void)));
  connect((QObject *) action_New_Game, SIGNAL(triggered(void)),
	  this, SLOT(slotNewGame(void)));

  /*
  ** Create the board.
  */

  if((qgl = new QGridLayout()) == NULL)
    {
      cerr << "Memory allocation error at line " << __LINE__ << "." << endl;
      exit(EXIT_FAILURE);
    }

  qgl->setSpacing(1);
  glpieces[0][0] = NULL;

  for(i = 0; i < NROWS; i++)
    for(j = 0; j < NCOLS; j++)
      {
	value = qrand() % (NROWS * NROWS);

	if(value == 0)
	  value += 1;
	else if(value > 64)
	  value = value / 2;

	if((i + j ) % 2 == 0)
	  color = QColor(205, 197, 191);
	else
	  color = QColor(139, 121, 94);

	glpieces[i][j] = new glpiece(NULL, glpieces[0][0], value, color);
	glpieces[i][j]->rotateBy(45 * 64, 45 * 64, -25 * 64);
	qgl->addWidget(glpieces[i][j], i, j);
      }

  boardframe->setLayout(qgl);
  resize(850, 850);
  show();
}

void maxit::slotNewGame(void)
{
  QFont font("Courier", 20, false);
  QString number = "";

  playerscore->display("0");
  opponentscore->display("0");

  for(int i = 0; i < NROWS; i++)
    for(int j = 0; j < NCOLS; j++)
      {
      }
}

bool maxit::isGameOver(void)
{
  bool gameOver = true;

  for(int i = 0; i < NROWS; i++)
    {
      if(!gameOver)
	break;
    }

  if(gameOver)
    if(playerscore->intValue() >
       opponentscore->intValue())
      QMessageBox::information(this, "Game Over", "You won!");
    else if(playerscore->intValue() <
	    opponentscore->intValue())
      QMessageBox::information(this, "Game Over", "The computer won!");
    else
      QMessageBox::information(this, "Game Over", "A tie!");

  return gameOver;
}

void maxit::slotAbout(void)
{
  QMessageBox mb(this);

  mb.setWindowTitle("Maximus: About");
  mb.setTextFormat(Qt::RichText);
  mb.setText("<html>Maximus Version 1.00.<br>"
	     "Copyright (c) Alexis Megas 2007, 2008.<br><br>"
	     "Please visit "
	     "<a href=\"http://maxit.sourceforge.net\">"
	     "http://maxit.sourceforge.net</a> for "
	     "project information."
	     "</html>");
  mb.setStandardButtons(QMessageBox::Ok);
  mb.exec();
}

void maxit::computerTurn(const int I, const int J)
{
  if(action_Easy->isChecked())
    easyMove(I, J);
}

void maxit::easyMove(const int I, const int J)
{
  (void) I;
  (void) J;

  for(int i = 0; i < NROWS; i++)
    {
    }
}
