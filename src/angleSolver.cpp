
#include "angleSolver.hpp"

// int main( int argc, const char** argv )
// {      

//     angleSolver angle_solver;
//     double alpha = angle_solver.get_ALPHA();
//     double thet = angle_solver.get_THET();
//     cout << "alpha is: "<<alpha*180/M_PI<<endl;
//     cout << "thet is: "<<thet*180/M_PI<<endl;
    
// }
double angleSolver::get_para_t(double vx0, double dis, double alpha)
{
    dis -= L * cos(alpha);
    double t = log(vx0 / (vx0 - (Co * dis))) / Co;
    return t;
}

double angleSolver::get_para_height(double vx0, double vy0, double dis, double alpha)
{
    double t = get_para_t(vx0, dis, alpha);
    double y = (-g / Co) * t + (vy0 + g / Co) / Co * (1 - exp(-Co * t));
    return y;
}

double angleSolver::get_str_v(double alpha)
{
    double v = sqrt(V * V - 2 * Miu * g * L * cos(alpha) - 2 * g * L * sin(alpha));
    return v;
}

double angleSolver::get_height(double alpha, double dis)
{
    double v = get_str_v(alpha);
    double vx0 = v * cos(alpha);
    double vy0 = v * sin(alpha);
    double y = get_para_height(vx0, vy0, dis, alpha);
    return y + H + L * sin(alpha);
}

double angleSolver::select_alpha(double dis)
{
    double alpha = 0, alpha_final = 0;
    double delta_final = 999, delta = 0;
    for (int i = 0; i < 180; i++)
    {
        alpha = alpha + 0.25 / 180 * M_PI;
        double height = get_height(alpha, dis);
        delta = abs(height - Z);
        if (delta < delta_final)
        {
            delta_final = delta;
            alpha_final = alpha;
        }
    }

    return alpha_final;
}

double angleSolver::get_thet(double X, double Y, double X_velocity, double Y_velocity, double thet0)
{
    double delta_thet = 0;
    double atanYoverX = 0;
    if (X = 0)
    {
        if (Y > 0)
            atanYoverX = M_PI_2;
        if (Y < 0)
            atanYoverX = -M_PI_2;
        if (Y = 0)
            ; //Y&&X can't be 0 at a same time
    }
    else
    {
        atanYoverX = atan(Y / X);
    }

    double atanYVoverXV = 0;
    if (X_velocity == 0)
    {
        if (Y_velocity > 0)
            atanYVoverXV = M_PI_2;
        if (Y_velocity < 0)
            atanYVoverXV = -M_PI_2;
        if (Y_velocity = 0)
            ;
    }
    else
    {
        atanYVoverXV = atan(Y_velocity / X_velocity);
    }

    double phi = M_PI - atanYoverX + atanYVoverXV;
    double V_object = sqrt(X_velocity * X_velocity + Y_velocity * Y_velocity);
    if (X_velocity == 0 && Y_velocity == 0)
    {
        delta_thet = 0;
    }
    else
    {
        delta_thet = asin(V_object * sin(phi) / V);
    }
    return thet0 + delta_thet;
}

double angleSolver::get_ALPHA()
{
    return angleSolver::select_alpha(dis);
}

double angleSolver::get_THET()
{
    double thet = 0;
    if (Y != 0)
    {
        thet = atan(X / Y);
    }
    else
    {
        thet = M_PI_2;
    }
    return angleSolver::get_thet(X, Y, X_velocity, Y_velocity, thet);
}

void angleSolver::setXYZ(double tempX, double tempY, double tempZ)
{
    X = tempX;
    Y = tempY;
    Z = tempZ;
}

void angleSolver::setRelativeV(double Vx, double Vy){
    X_velocity = Vx; // to the right
    Y_velocity = Vy; // to the front
}

