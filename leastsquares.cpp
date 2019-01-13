#include "leastsquares.h"


LeastSquares::LeastSquares()
{
    ;
}

LeastSquares::LeastSquares(Mat &input)
{
    float avgX(0);
    float avgY(0);
    double sumProdXY(0);
    double sumXSqrd(0);

    average(input, avgX, avgY);
    pixelVector data = extractPoints(input);

    for(auto pair=data.begin();pair!=data.end();pair++){
        // Xm = xi - mean(X)
        float Xm = static_cast<float>(pair->first)-avgX;
        // Ym = yi - mean(Y)
        float Ym = static_cast<float>(pair->second)-avgY;
        // product = Xm * Ym
        float product = Xm*Ym;

        sumProdXY += static_cast<double>(product);
        sumXSqrd += pow(Xm,2);
    }

    // slope = sum(Xm, Ym)/sum(Xm^2)
    k = sumProdXY/sumXSqrd;
    // intercept = mean(Y) − slope * mean(X)
    q = static_cast<double>(avgY) - (k*static_cast<double>(avgX));

}

LeastSquares::LeastSquares(Contour contour)
{
    int N(0);
    vector<double> x2;
    vector<double> y2;
    vector<double> xy;
    double kd, x, y, X2, Y2, XY;
    double X(0), Y(0);

    for(auto pixel=contour.begin();pixel!=contour.end();pixel++){
        N++;
        x = pixel->x;
        y = pixel->y;
        xy.push_back(x*y);
        x2.push_back(x*x);
        y2.push_back(y*y);
        X += x;
        Y += y;
    }
    XY = sum<double>(xy);
    X2 = sum<double>(x2);
    Y2 = sum<double>(y2);

    /*
     *      (NΣxy − ΣxΣy)
     * k = ----------------
     *      N(Σx2) − (Σx)^2
     */

    k = (N*XY - X*Y) / (N*X2 - (X*X));

    /*
     *      (NΣxy − ΣxΣy)
     * k' = ----------------
     *      N(Σy2) − (Σy)^2
     */

    kd = (N*XY - X*Y) / (N*Y2 - (Y*Y));

    /*
     *      Σy − m(Σx)
     * q = ------------
     *          N
     */
    q = (Y - k*X) / N;

    /* coefficient of determination
     *
     * R2 = k.k'
     */

    R2 =k*kd;
}

double LeastSquares::getX(double y)
{
    return (y-q) / k;
}

double LeastSquares::getY(double x)
{
    return (k*x + q);
}

