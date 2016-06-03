#ifndef __MODEL__ 
#define __MODEL__ 
#include <Windows.h> 
/**
* \brief The structure of the ball used to describe the model of the ball
*
* \param X current coordinate along the horizontal axis
*
* \param Y current coordinate on the vertical axis
*
* \param dX The rate of change of coordinates on the X axis
*
* \param dY The rate of change along the coordinate axis Y
*
* \param r The radius of the ball
*
* \code
* struct Ball
* {
* double X,Y;
* double dX,dY;
* double r;
* };
* \endcode
*/

struct Ball
{
	double X, Y;
	double dX, dY;
	double r;
};
/**
* \brief The structure used to create the Player 1
*
* Player also have structure like structure of the ball,
* because players depicted circles
*
* Used variables:
*
* \param X Player 1 Start position
*
* \param v speed of Player 1
*
* \param a Player 1 acceleration
*
* \param x current coordinate along the horizontal axis
*
* \param y current coordinate on the vertical axis
*
* \param vx The rate of change of coordinates on the X axis
*
* \param vy The rate of change along the coordinate axis Y
*
* \param isSelected It indicates that this player is selected
*
*
* \code
* struct Player1
* {
* double X;
* double v, a;
* double x, y;
* double vx, vy;
* bool isSelected;
* };
* \endcode
*/

struct Player1
{
	double X;
	double v, a;

	double x, y;
	double vx, vy;

	bool isSelected;


};
/**
* \brief The structure used to create the Player 1
*
* Player also have structure like structure of the ball,
* because players depicted circles
*
* Used variables:
*
* \param q speed of Player 2
*
* \param l Player 2 acceleration
*
* \param z current coordinate along the horizontal axis
*
* \param t current coordinate on the vertical axis
*
* \param vz The rate of change of coordinates on the X axis
*
* \param vt The rate of change along the coordinate axis Y
*
* \param isSelected It indicates that this player is selected
*
*
* \code
* struct Player2
* {
* double q,l;
* double z, t;
* double vz, vt;
* bool isSelected;
* };
* \endcode
*/
struct Player2
{
	double q, l;
	double z, t;
	double vz, vt;

	bool isSelected;
};
Ball CreateBall(RECT r);
#endif
