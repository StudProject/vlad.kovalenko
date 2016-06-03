#include "Model.h" 
/**
* \brief function creates a ball of a certain size
* This model we use for players to.
*
* Variables:
* \param result Object with the structure of the ball
* \param w change the width of the ball coordinates
* \param h height change ball Coordinate
* We set the parameters of the ball and the speed of the axes
* Code
* \code
*
* Ball CreateBall(RECT r)
* {
* Ball result;
* int w = r.right - r.left;
* int h = r.bottom - r.top;
* result.X = 600;
* result.Y = 50;
* result.dX = 150;
* result.dY = 10;
* result.r = 30;
* return result;
* }
* \endcode
*/
Ball CreateBall(RECT r)
{
	Ball result;
	int w = r.right - r.left;
	int h = r.bottom - r.top;
	result.X = 600;
	result.Y = 50;
	result.dX = 150;
	result.dY = 10;
	result.r = 30;
	return result;
}