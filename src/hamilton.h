#include <cmath>

// Please read more at
// https://www.sciencedirect.com/science/article/pii/0024379595005439?via%3Dihub
// Cheers

struct Hamilton 
{
    public:
        double w; 
        double i; 
        double j;
        double k;

        Hamilton(double w, double i, double j, double k)
        {
            this->w = w;
            this->i = i;
            this->j = j;
            this->k = k;
        }

        Hamilton add(const Hamilton &a)
        {
            return Hamilton(w + a.w, i + a.i, j + a.j, k + a.j);
        }

        Hamilton multiply(const Hamilton &coeff)
        {
            // Quaternion multiplication table
            return Hamilton(
                w * coeff.w - i * coeff.i - j * coeff.j - k * coeff.k,
                w * coeff.i + i * coeff.w + j * coeff.k - k * coeff.j,
                w * coeff.j - i * coeff.k + j * coeff.w + k * coeff.i,
                w * coeff.k + i * coeff.j - j * coeff.i + k * coeff.w);
        }

        Hamilton exp()
        {
            double magnitude = std::sqrt(i * i + j * j + k * k);
            
            if(magnitude == 0) 
            {
                return Hamilton(std::exp(w), 0, 0, 0);
            }

            double sinofmagnitude = sin(magnitude) / magnitude;
            double ew = std::exp(w);
            return Hamilton(
                ew * std::cos(magnitude), 
                ew * i * sinofmagnitude, 
                ew * j * sinofmagnitude, 
                ew * k * sinofmagnitude);
        }

        Hamilton cos()
        {
            double mag = std::sqrt(i * i + j * j + k * k);
            if (mag == 0) 
            {
                return Hamilton(std::cos(w), 0, 0, 0);
            }

            double t = -std::sin(w) * std::sinh(mag) / mag;
            return Hamilton(std::cos(w) * std::cosh(mag), t * i, t * j, t * k);
        }
    
        double magnitude()
        {
            return std::sqrt(w * w + i * i + j * j + k * k); 
        }
};