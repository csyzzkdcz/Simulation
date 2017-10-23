#include "ODEsolver.h"
#include <Eigen>

ODEsolver::ODEsolver(void)
{
}

ODEsolver::~ODEsolver(void)
{
}

void ODEsolver::Euler( VectorXd a, VectorXd &v, VectorXd &x )
{
	v += a * UNIT_TIME;
	x += v * UNIT_TIME;
}

void ODEsolver::Runge_Kutta( VectorXd a,VectorXd &v,VectorXd &x )
{

}

void ODEsolver::Verlet( VectorXd a,VectorXd &v,VectorXd &x )
{
	x += v * UNIT_TIME + a * pow(UNIT_TIME,2);
	v += a * UNIT_TIME;
}

void ODEsolver::Newton_Raphson( VectorXd a,VectorXd &v,VectorXd &x,MatrixXd K )
{
}