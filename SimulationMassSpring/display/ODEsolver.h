#pragma once
#include <Eigen>

#define UNIT_TIME 0.01

using Eigen::VectorXd;
using Eigen::MatrixXd;

class ODEsolver
{
public:
	ODEsolver(void);
	~ODEsolver(void);

public:
	void Euler( VectorXd a,VectorXd &v,VectorXd &x );
	void Runge_Kutta( VectorXd a,VectorXd &v,VectorXd &x );
	void Verlet( VectorXd a,VectorXd &v,VectorXd &x );
	void Newton_Raphson( VectorXd a,VectorXd &v,VectorXd &x,MatrixXd K );

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};

